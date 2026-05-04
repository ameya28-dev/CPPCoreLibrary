#include "core/http/codes/network_status.hpp"

#include <cassert>
#include <optional>

core::NetworkErrorStatus core::toNetworkErrorStatus(const DWORD error) {
    switch (error) {
#define X(name, code, str, desc) \
    case code:                   \
        return core::NetworkErrorStatus::name;
        NETWORK_ERROR_LIST(X)
#undef X
    default:
        return NetworkErrorStatus::Unknown;
    }
}

std::string core::toString(const NetworkErrorStatus code) {
    switch (code) {
#define X(name, code, str, desc)         \
    case core::NetworkErrorStatus::name: \
        return str;
        NETWORK_ERROR_LIST(X)
#undef X
    default:
        return "Unknown";
    }
}

std::string core::getDescription(const NetworkErrorStatus code) {
    switch (code) {
#define X(name, code, str, desc)         \
    case core::NetworkErrorStatus::name: \
        return desc;
        NETWORK_ERROR_LIST(X)
#undef X
    default:
        return "An unspecified error occurred.";
    }
}

bool core::isTransient(const std::optional<NetworkErrorStatus> netErr, std::optional<HttpStatus> status) {
    assert((netErr != std::nullopt && status != std::nullopt)
           && "Both HTTP Status and system level error codes can't be null");

    // 1. Check Network/Transport errors
    if (netErr) {
        if (netErr.value() == NetworkErrorStatus::Timeout || netErr.value() == NetworkErrorStatus::CannotConnect
            || netErr.value() == NetworkErrorStatus::ConnectionAborted
            || netErr.value() == NetworkErrorStatus::ResendRequest) {
            return true;
        }
    }

    // 2. Check HTTP status codes (5xx)
    if (status) {
        if (const int code = static_cast<int>(status.value()); code >= 500 && code <= 599) {
            return true;
        }
    }

    return false;
}

int core::getNumericCodeOfError(const NetworkErrorStatus code) {
    switch (code) {
#define X(name, code, str, desc)         \
    case core::NetworkErrorStatus::name: \
        return static_cast<int>(code);
        NETWORK_ERROR_LIST(X)
#undef X
    default:
        return -1;
    }
}
