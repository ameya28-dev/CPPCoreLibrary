#ifndef CORELIBRARY_INCLUDE_CORE_HTTP_TYPES_TYPES_HPP
#define CORELIBRARY_INCLUDE_CORE_HTTP_TYPES_TYPES_HPP

#include <type_traits>
#include <variant>

#include "core/http/codes/http_status.hpp"
#include "core/http/codes/network_status.hpp"
#include <nlohmann/json.hpp>

namespace core {
    struct Empty {};

    inline void from_json(const nlohmann::json&, Empty) {}

    struct NetworkError {
        std::string message;
        NetworkErrorStatus code;
    };

    struct NetworkResult {
        std::string body;
        HttpStatus status;
    };

    using NetworkResponse = std::variant<NetworkResult, NetworkError>;

    std::string getErrorMessage(const NetworkResponse& response);

    std::string getRawBody(const NetworkResponse& response);

    int getStatusCode(const NetworkResponse& response);

    template <typename T = std::string>
    struct Success {
        T body;
        int status;
    };

    enum struct ApiError { SystemError, HTTP, InvalidJson, MappingError, MaxRetriesReached, Unknown };

    constexpr const char* getDescription(ApiError error) {
        switch (error) {
        case ApiError::SystemError:
            return "System Level Error";
        case ApiError::HTTP:
            return "Error served by the server";
        case ApiError::InvalidJson:
            return "Tried to parse Invalid JSON";
        case ApiError::MappingError:
            return "Invalid mapping of JSON Object";
        case ApiError::MaxRetriesReached:
            return "Retries exhausted";
        case ApiError::Unknown:
            return "Unknown";
        default:
            return "Unknown";
        }
    }

    template <typename T = std::string>
    struct ApiFailure {
        T body;
        std::string message;
        int status;
    };

    struct OtherFailure {
        std::string message;
        ApiError error;
        int code;
    };

    template <typename T, typename E = std::string>
    using ApiResult = std::variant<Success<T>, ApiFailure<E>, OtherFailure>;

} // namespace core

#endif // CORELIBRARY_INCLUDE_CORE_HTTP_TYPES_TYPES_HPP
