#include "core/http/types/types.hpp"

#include "core/utils/utils.hpp"

size_t core::CaseInsensitiveHash::operator()(const std::string_view key) const {
    return caseInsensitiveHash(key);
}

bool core::CaseInsensitiveEqual::operator()(const std::string_view a, const std::string_view b) const {
    return isEqualCaseInsensitive(a, b);
}

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

int core::getRawStatusCode(const NetworkResponse& response) {
    if (const auto error = std::get_if<NetworkError>(&response)) {
        return getNumericCodeOfError(error->code);
    }

    if (const auto [body, status] = std::get<NetworkResult>(response);
        status != HttpStatus::Accepted && status != HttpStatus::Created && status != HttpStatus::Ok) {
        return static_cast<int>(status);
    }

    return -1;
}
