#ifndef CORELIBRARY_INCLUDE_CORE_WIN_HTTP_UTILS_UTILS_HPP
#define CORELIBRARY_INCLUDE_CORE_WIN_HTTP_UTILS_UTILS_HPP

#include <string>
#include <string_view>

#include <windows.h>

#include "core/win_http/types/types.hpp"

namespace core {


    // Fast ASCII toLower
    char toLower(char c);

    size_t caseInsensitiveHash(std::string_view);

    bool isEqualCaseInsensitive(std::string_view, std::string_view);

    void printError(const std::string&, DWORD);

    std::wstring toWideString(const std::string&);

    std::string urlEncode(const std::string&);

    std::wstring generateURLEndpoint(const std::wstring&, const Params&);

} // namespace core

#endif // CORELIBRARY_INCLUDE_CORE_WIN_HTTP_UTILS_UTILS_HPP
