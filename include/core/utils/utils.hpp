#ifndef CORELIBRARY_INCLUDE_CORE_WIN_HTTP_UTILS_UTILS_HPP
#define CORELIBRARY_INCLUDE_CORE_WIN_HTTP_UTILS_UTILS_HPP

#include <map>
#include <string>
#include <string_view>
#include <unordered_map>

#include <windows.h>

namespace core {
    struct CaseInsensitiveHash {
        size_t operator()(std::string_view) const;
    };

    struct CaseInsensitiveEqual {
        bool operator()(std::string_view, std::string_view) const;
    };

    using Headers = std::unordered_multimap<std::string, std::string, CaseInsensitiveHash, CaseInsensitiveEqual>;
    using Params  = std::multimap<std::string, std::string>;

    // Fast ASCII toLower
    char toLower(char c);

    size_t caseInsensitiveHash(std::string_view);

    bool isEqualCaseInsensitive(std::string_view, std::string_view);

    void printError(const std::string&, DWORD);

    std::string urlEncode(const std::string&);

#if WIN32
    std::wstring toWideString(const std::string&);

    std::wstring generateURLEndpoint(const std::wstring&, const std::multimap<std::string, std::string>&);
#endif

} // namespace core

#endif // CORELIBRARY_INCLUDE_CORE_WIN_HTTP_UTILS_UTILS_HPP
