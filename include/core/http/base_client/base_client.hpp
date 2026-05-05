#ifndef CORELIBRARY_INCLUDE_CORE_HTTP_BASE_CLIENT_BASE_CLIENT_HPP
#define CORELIBRARY_INCLUDE_CORE_HTTP_BASE_CLIENT_BASE_CLIENT_HPP

#include <cassert>
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
    inline static UniformInt generator{constants::Jitter::Min, constants::Jitter::Max};

    class BaseClient {
    public:
        explicit BaseClient(const std::string& agent, const std::string& host, bool isHTTPS = true);

        [[nodiscard]] auto IsConstructed() const -> bool;

        auto SetVerbose(bool) -> void;

        auto SetRetries(int) -> void;

        auto SetTimeouts(std::chrono::milliseconds connect, std::chrono::milliseconds send,
            std::chrono::milliseconds receive, std::chrono::milliseconds resolve) const -> void;

        template <typename Resp = std::string, typename Err = std::string>
        auto Get(const std::string& path, const Params& params = {}, const Headers& headers = {})
            -> std::enable_if_t<IsJsonDeserializable<Resp>::value && IsJsonDeserializable<Err>::value,
                ApiResult<Resp, Err>>;

        template <typename Req = std::string, typename Resp = std::string, typename Err = std::string>
        auto Post(const std::string& path, const Req& body, const Headers& headers = {})
            -> std::enable_if_t<IsJsonDeserializable<Resp>::value && IsJsonDeserializable<Err>::value
                                    && IsJsonSerializable<Req>::value,
                ApiResult<Resp, Err>>;

        template <typename Req = std::string, typename Resp = std::string, typename Err = std::string>
        auto Put(const std::string& path, const Req& body, const Headers& headers = {})
            -> std::enable_if_t<IsJsonDeserializable<Resp>::value && IsJsonDeserializable<Err>::value
                                    && IsJsonSerializable<Req>::value,
                ApiResult<Resp, Err>>;

        template <typename Req = std::string, typename Resp = std::string, typename Err = std::string>
        auto Patch(const std::string& path, const Req& body, const Headers& headers = {})
            -> std::enable_if_t<IsJsonDeserializable<Resp>::value && IsJsonDeserializable<Err>::value
                                    && IsJsonSerializable<Req>::value,
                ApiResult<Resp, Err>>;

        template <typename Err = std::string>
        auto Delete(const std::string& path, const Params& params = {}, const Headers& headers = {})
            -> ApiResult<Empty, Err>;

    private:
        NetworkResponse _sendRequest(
            HTTPMethod method, const std::string& path, const Params& params, const Headers& headers);

        template <typename Req>
        auto _sendRequest(HTTPMethod method, const std::string& path, const Headers& headers, const Req& payload)
            -> std::enable_if_t<IsJsonSerializable<Req>::value, NetworkResponse>;

        template <typename T = std::string, typename Err = std::string, typename Func>
        auto _executeWithRetry(Func&&, const std::string&) -> std::variant<Success<T>, Failure<Err>>;

        template <typename T = std::string, typename Err = std::string>
        auto _parseToVariant(NetworkResponse&& response) -> ApiResult<T, Err>;

    private:
#if WIN32
        NetworkHandler _handler;
#endif
        int _retryCount     = constants::Defaults::RetryCount;
        bool _verbose       = constants::Defaults::Verbose;
        bool _isOKConstruct = false;
    };

    template <typename Resp, typename Err>
    auto BaseClient::Get(const std::string& path, const Params& params, const Headers& headers)
        -> std::enable_if_t<IsJsonDeserializable<Resp>::value && IsJsonDeserializable<Err>::value,
            ApiResult<Resp, Err>> {
        assert(_isOKConstruct && "Cannot request over network as connection failed");
        return _executeWithRetry<Resp, Err>(
            [&](const std::string& p) { return _sendRequest(HTTPMethod::Get, p, params, headers); }, path);
    }

    template <typename Req, typename Resp, typename Err>
    auto BaseClient::Post(const std::string& path, const Req& body, const Headers& headers)
        -> std::enable_if_t<IsJsonDeserializable<Resp>::value && IsJsonDeserializable<Err>::value
                                && IsJsonSerializable<Req>::value,
            ApiResult<Resp, Err>> {
        assert(_isOKConstruct && "Cannot request over network as connection failed");
        return _executeWithRetry<Resp, Err>(
            [&](const std::string& p) { return _sendRequest<Req>(HTTPMethod::Post, p, headers, body); }, path);
    }

    template <typename Req, typename Resp, typename Err>
    auto BaseClient::Put(const std::string& path, const Req& body, const Headers& headers)
        -> std::enable_if_t<IsJsonDeserializable<Resp>::value && IsJsonDeserializable<Err>::value
                                && IsJsonSerializable<Req>::value,
            ApiResult<Resp, Err>> {
        assert(_isOKConstruct && "Cannot request over network as connection failed");
        return _executeWithRetry<Resp, Err>(
            [&](const std::string& p) { return _sendRequest<Req>(HTTPMethod::Put, p, headers, body); }, path);
    }

    template <typename Req, typename Resp, typename Err>
    auto BaseClient::Patch(const std::string& path, const Req& body, const Headers& headers)
        -> std::enable_if_t<IsJsonDeserializable<Resp>::value && IsJsonDeserializable<Err>::value
                                && IsJsonSerializable<Req>::value,
            ApiResult<Resp, Err>> {
        assert(_isOKConstruct && "Cannot request over network as connection failed");
        return _executeWithRetry<Resp, Err>(
            [&](const std::string& p) { return _sendRequest<Req>(HTTPMethod::Patch, p, headers, body); }, path);
    }

    template <typename Err>
    auto BaseClient::Delete(const std::string& path, const Params& params, const Headers& headers)
        -> ApiResult<Empty, Err> {
        assert(_isOKConstruct && "Cannot request over network as connection failed");
        return _executeWithRetry<Empty, Err>(
            [&](const std::string& p) { return _sendRequest(HTTPMethod::Delete, p, params, headers); }, path);
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
#else
        return NetworkError{"WinHTTP is not supported on this platform", NetworkErrorStatus::Unknown};
#endif
    }

    template <typename T, typename Err, typename Func>
    auto BaseClient::_executeWithRetry(Func&& func, const std::string& path) -> ApiResult<T, Err> {
        NetworkResponse res{};
        int attempts = 0;

        while (true) {
            res = func(path);
            /// retry only if HttpStatus and ErrorCode are transient
            if (const auto error = std::get_if<NetworkError>(&res)) {
                if (!isTransient(error->code)) {
                    return _parseToVariant<T, Err>(std::move(res));
                }
            }

            if (const auto [body, status] = std::get<NetworkResult>(res); !isTransient(status)) {
                return _parseToVariant<T, Err>(std::move(res));
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

    template <typename T, typename Err>
    auto BaseClient::_parseToVariant(NetworkResponse&& response) -> ApiResult<T, Err> {
        if (const auto error = std::get_if<NetworkError>(&response)) {
            return Failure<>{"", error->message, ApiError::SystemError, getNumericCodeOfError(error->code)};
        }

        const auto [body, status] = std::get<NetworkResult>(response);
        try {
            if (status > HttpStatus::ImUsed || status < HttpStatus::Ok) {
                return Failure<>{
                    core::tryParse<Err>(body), getDescription(status), ApiError::HTTP, static_cast<int>(status)};
            }

            if constexpr (std::is_same_v<T, Empty>) {
                return Success<Empty>{Empty{}, static_cast<int>(status)};
            }

            return Success<T>{core::tryParse<T>(body), static_cast<int>(status)};
        } catch (const nlohmann::json::parse_error& ex) {
            return Failure<>{body, ex.what(), ApiError::InvalidJson, static_cast<int>(status)};
        } catch (const std::exception& ex) {
            return Failure<>{body, ex.what(), ApiError::MappingError, static_cast<int>(status)};
        }
    }

} // namespace core


#endif // CORELIBRARY_INCLUDE_CORE_HTTP_BASE_CLIENT_BASE_CLIENT_HPP
