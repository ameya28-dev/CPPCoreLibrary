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

    int getRawStatusCode(const NetworkResponse& response);

    template <typename T = std::string>
    struct Success {
        T body;
        int status;
    };

    enum struct ApiError { SystemError, HTTP, InvalidJson, MappingError, MaxRetriesReached, Unknown };

    template <typename T = std::string>
    struct Failure {
        T rawBody;
        std::string message;
        ApiError error;
        int rawStatus;
    };

    template <typename T, typename E = std::string>
    using ApiResult = std::variant<Success<T>, Failure<E>>;


} // namespace core

#endif // CORELIBRARY_INCLUDE_CORE_HTTP_TYPES_TYPES_HPP
