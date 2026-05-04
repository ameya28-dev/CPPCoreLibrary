#include "core/win_http/types/types.hpp"

#if WIN32

void core::WinHttpHandleDeleter::operator()(const pointer h) const {
    if (h != nullptr) {
        WinHttpCloseHandle(h);
    }
}

const wchar_t* core::toWinHttpVerb(const core::HTTPMethod method) {
    switch (method) {
#define AS_HTTP_METHOD_VALUE(name, value) \
    case HTTPMethod::name:                \
        return value;
        HTTP_METHOD_ENUM_LIST(AS_HTTP_METHOD_VALUE)
#undef AS_HTTP_METHOD_VALUE

    default:
        return L"GET";
    }
}

#endif
