#include "win_http/types/types.hpp"

#include "win_http/utils/utils.hpp"

size_t core::CaseInsensitiveHash::operator()(const std::string_view key) const {
    return caseInsensitiveHash(key);
}

bool core::CaseInsensitiveEqual::operator()(std::string_view a, std::string_view b) const {
    return isEqualCaseInsensitive(a, b);
}

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
