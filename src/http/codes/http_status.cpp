#include "core/http/codes/http_status.hpp"

const char* core::toString(const HttpStatus status) {
    switch (status) {
#define AS_HTTP_STATUS_NAMES(name, value, desc) \
    case HttpStatus::name:                      \
        return #name;
        HTTP_STATUS_LIST(AS_HTTP_STATUS_NAMES)
#undef AS_HTTP_STATUS_NAMES
    default:
        return "Unknown";
    }
}

const char* core::getDescription(const HttpStatus status) {
    switch (status) {
#define AS_HTTP_STATUS_DESCRIPTION(name, value, desc) \
    case HttpStatus::name:                            \
        return desc;
        HTTP_STATUS_LIST(AS_HTTP_STATUS_DESCRIPTION)
#undef AS_HTTP_STATUS_DESCRIPTION
    default:
        return "Unknown";
    }
}

std::array<core::HttpStatus, core::HttpStatusCount> core::getHttpStatusList() {
    return httpStatusArray;
}
