#ifndef CORELIBRARY_INCLUDE_CORE_HTTP_BASE_CLIENT_BASE_CLIENT_HPP
#define CORELIBRARY_INCLUDE_CORE_HTTP_BASE_CLIENT_BASE_CLIENT_HPP

#include <chrono>
#include <fmt/base.h>
#include <fmt/chrono.h>
#include <string>
#include <thread>
#include <type_traits>

#include "core/http/constants/constants.hpp"
#include "core/http/types/types.hpp"
#include "core/logger/manager/manager.hpp"
#include "core/random/random.hpp"
#include "core/utils/utils.hpp"

#if WIN32
#include "core/win_http/handler/network.hpp"
#endif

namespace core {

    static UniformInt generator{constants::Jitter::Min, constants::Jitter::Max};

    class BaseClient {
    public:
        explicit BaseClient(const std::string& agent, const std::string& host, bool isHTTPS = true);

        [[nodiscard]] auto IsConstructed() const -> bool;

        auto SetVerbose(bool) -> void;

        auto SetRetries(int) -> void;

        auto SetTimeouts(std::chrono::milliseconds connect, std::chrono::milliseconds send,
            std::chrono::milliseconds receive, std::chrono::milliseconds resolve) const -> void;

        template <typename Resp>
        auto Get(const std::string& path, const Params& params = {}, const Headers& headers = {})
            -> std::enable_if_t<IsJsonDeserializable<Resp>::value, ApiResult<Resp>>;

        template <typename Resp, typename Req>
        auto Post(const std::string& path, const Req& body, const Headers& headers = {})
            -> std::enable_if_t<IsJsonDeserializable<Resp>::value && IsJsonSerializable<Req>::value, ApiResult<Resp>>;

        template <typename Resp, typename Req>
        auto Put(const std::string& path, const Req& body, const Headers& headers = {})
            -> std::enable_if_t<IsJsonDeserializable<Resp>::value && IsJsonSerializable<Req>::value, ApiResult<Resp>>;

        template <typename Resp, typename Req>
        auto Patch(const std::string& path, const Req& body, const Headers& headers = {})
            -> std::enable_if_t<IsJsonDeserializable<Resp>::value && IsJsonSerializable<Req>::value, ApiResult<Resp>>;

        auto Delete(const std::string& path, const Params& params = {}, const Headers& headers = {})
            -> ApiResult<Empty>;

    private:
        NetworkResponse _sendRequest(
            HTTPMethod method, const std::string& path, const Params& params, const Headers& headers);

        template <typename Req>
        auto _sendRequest(HTTPMethod method, const std::string& path, const Headers& headers, const Req& payload)
            -> std::enable_if_t<IsJsonSerializable<Req>::value, NetworkResponse>;

        template <typename T, typename Func>
        auto _executeWithRetry(Func&&, const std::string&) -> ApiResult<T>;

        template <typename T>
        auto _parseToVariant(NetworkResponse&& response) -> ApiResult<T>;

    private:
#if WIN32
        NetworkHandler _handler;
#endif
        int _retryCount     = constants::Defaults::RetryCount;
        bool _verbose       = constants::Defaults::Verbose;
        bool _isOKConstruct = false;
    };

    template <typename Resp>
    auto BaseClient::Get(const std::string& path, const Params& params, const Headers& headers)
        -> std::enable_if_t<IsJsonDeserializable<Resp>::value, ApiResult<Resp>> {
        assert(_isOKConstruct && "Cannot request over network as connection failed");
        return _executeWithRetry<Resp>(
            [&](const std::string& p) { return _sendRequest(HTTPMethod::Get, p, params, headers); }, path);
    }

    template <typename Resp, typename Req>
    auto BaseClient::Post(const std::string& path, const Req& body, const Headers& headers)
        -> std::enable_if_t<IsJsonDeserializable<Resp>::value && IsJsonSerializable<Req>::value, ApiResult<Resp>> {
        assert(_isOKConstruct && "Cannot request over network as connection failed");
        return _executeWithRetry<Resp>(
            [&](const std::string& p) { return _sendRequest<Req>(HTTPMethod::Post, p, headers, body); }, path);
    }

    template <typename Resp, typename Req>
    auto BaseClient::Put(const std::string& path, const Req& body, const Headers& headers)
        -> std::enable_if_t<IsJsonDeserializable<Resp>::value && IsJsonSerializable<Req>::value, ApiResult<Resp>> {
        assert(_isOKConstruct && "Cannot request over network as connection failed");
        return _executeWithRetry<Resp>(
            [&](const std::string& p) { return _sendRequest<Req>(HTTPMethod::Put, p, headers, body); }, path);
    }

    template <typename Resp, typename Req>
    auto BaseClient::Patch(const std::string& path, const Req& body, const Headers& headers)
        -> std::enable_if_t<IsJsonDeserializable<Resp>::value && IsJsonSerializable<Req>::value, ApiResult<Resp>> {
        assert(_isOKConstruct && "Cannot request over network as connection failed");
        return _executeWithRetry<Resp>(
            [&](const std::string& p) { return _sendRequest<Req>(HTTPMethod::Patch, p, headers, body); }, path);
    }

    template <typename Req>
    auto BaseClient::_sendRequest(const HTTPMethod method, const std::string& path, const Headers& headers,
        const Req& payload) -> std::enable_if_t<IsJsonSerializable<Req>::value, NetworkResponse> {
        assert(_isOKConstruct && "Cannot request over network as connection failed");
#if WIN32
        try {
            const auto wPath = toWideString(path);
            _handler.Request(method, wPath.c_str());
            _handler.AddHeaders(headers);
            _handler.SendRequestWithBody(nlohmann::json(payload).dump());
            _handler.ReceiveResponse();
            const auto [body, status] = _handler.ReadResponse();
            return NetworkResult{body, static_cast<HttpStatus>(status)};
        } catch (const std::system_error& ex) {
            return NetworkError{ex.what(), toNetworkErrorStatus(ex.code().value())};
        } catch (const std::runtime_error& ex) {
            return NetworkError{ex.what(), NetworkErrorStatus::InternalError};
        }
#endif
    }

    template <typename T, typename Func>
    ApiResult<T> BaseClient::_executeWithRetry(Func&& func, const std::string& path) {
        NetworkResponse res{};
        int attempts = 0;

        while (true) {
            res = func(path);
            /// retry only if HttpStatus and ErrorCode are transient
            if (const auto error = std::get_if<NetworkError>(&res); error && !isTransient(error->code)) {
                return Failure<>{"", error->message, ApiError::SystemError, getNumericCodeOfError(error->code)};
            }

            if (const auto result = std::get<NetworkResult>(res); !isTransient(result.status)) {
                return _parseToVariant<T>(result);
            }


            if (attempts >= _retryCount) {
                return Failure<>{getRawBody(res),
                    fmt::format("Retry limit ({}) reached. Final cause: {}", _retryCount, getErrorMessage(res)),
                    ApiError::MaxRetriesReached, getRawStatusCode(res)};
            }
            attempts++;
            auto waitDuration = std::chrono::seconds(1 << (attempts - 1)) + std::chrono::microseconds(generator.next());
            if (_verbose) {
                LOG_INFO("[RETRY] Attempt {}/{} failed. Waiting {}ms...\n", attempts, _retryCount, waitDuration);
            }
            std::this_thread::sleep_for(waitDuration);
        }
    }

    template <typename T>
    auto BaseClient::_parseToVariant(NetworkResponse&& response) -> ApiResult<T> {
        if (const auto error = std::get_if<NetworkError>(&response)) {
            return Failure<>{"", error->message, ApiError::SystemError, getNumericCodeOfError(error->code)};
        }

        const auto [body, status] = std::get<NetworkResult>(response);
        if (status > HttpStatus::ImUsed || status < HttpStatus::Ok) {
            return Failure<>{body, getDescription(status), ApiError::HTTP, static_cast<int>(status)};
        }

        try {
            if constexpr (std::is_same_v<T, Empty>) {
                return Success<Empty>{Empty{}, static_cast<int>(status)};
            }

            if constexpr (std::is_same_v<T, std::string>) {
                return Success<T>{body, static_cast<int>(status)};
            }

            nlohmann::json j = body.empty() ? nlohmann::json::object() : nlohmann::json::parse(body);
            return Success<T>{j.get<T>(), static_cast<int>(status)};
        } catch (const nlohmann::json::parse_error& ex) {
            return Failure<>{body, ex.what(), ApiError::InvalidJson, static_cast<int>(status)};
        } catch (const std::exception& ex) {
            return Failure<>{body, ex.what(), ApiError::MappingError, static_cast<int>(status)};
        }
    }

} // namespace core


#endif // CORELIBRARY_INCLUDE_CORE_HTTP_BASE_CLIENT_BASE_CLIENT_HPP
