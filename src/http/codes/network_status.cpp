#include "core/http/codes/network_status.hpp"

#include <cassert>

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

bool core::isTransient(const NetworkErrorStatus netErr) {
    if (netErr == NetworkErrorStatus::Timeout || netErr == NetworkErrorStatus::CannotConnect
        || netErr == NetworkErrorStatus::ConnectionAborted || netErr == NetworkErrorStatus::ResendRequest) {
        return true;
    }

    return false;
}

bool core::isTransient(HttpStatus status) {
    if (const int code = static_cast<int>(status); code >= 500 && code <= 599) {
        return true;
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
