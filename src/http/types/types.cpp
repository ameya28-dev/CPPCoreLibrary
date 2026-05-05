#include "core/http/types/types.hpp"

void core::from_json(const nlohmann::json&, Empty) {}

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

const char* core::getDescription(const ApiError error) {
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
