#include "core/http/types/types.hpp"

std::string core::getErrorMessage(const NetworkResponse& response) {
    if (const auto error = std::get_if<NetworkError>(&response)) {
        return error->message;
    }

    if (const auto [body, status] = std::get<NetworkResult>(response);
        status != HttpStatus::Accepted && status != HttpStatus::Created && status != HttpStatus::Ok) {
        return body;
    }
    return "";
}

std::string core::getRawBody(const NetworkResponse& response) {
    if (std::get_if<NetworkError>(&response)) {
        return "";
    }

    if (const auto [body, status] = std::get<NetworkResult>(response);
        status != HttpStatus::Accepted && status != HttpStatus::Created && status != HttpStatus::Ok) {
        return body;
    }
    return "";
}

int core::getStatusCode(const NetworkResponse& response) {
    if (const auto error = std::get_if<NetworkError>(&response)) {
        return getNumericCodeOfError(error->code);
    }

    if (const auto [body, status] = std::get<NetworkResult>(response);
        status != HttpStatus::Accepted && status != HttpStatus::Created && status != HttpStatus::Ok) {
        return static_cast<int>(status);
    }

    return -1;
}
