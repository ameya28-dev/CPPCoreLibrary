#ifndef CORELIBRARY_INCLUDE_CORE_WIN_HTTP_TYPES_TYPES_HPP
#define CORELIBRARY_INCLUDE_CORE_WIN_HTTP_TYPES_TYPES_HPP

#include <memory>
#include <string>

#if WIN32
// clang-format off
#include <windows.h>
#include <winhttp.h>
// clang-format on
#endif

namespace core {

#if WIN32
    struct WinHttpHandleDeleter {
        using pointer = HINTERNET;

        void operator()(pointer h) const;
    };

    using ScopedHInternet = std::unique_ptr<HINTERNET, WinHttpHandleDeleter>;
#endif

#define HTTP_METHOD_ENUM_LIST(X) X(Get, L"GET") X(Post, L"POST") X(Put, L"PUT") X(Patch, L"PATCH") X(Delete, L"DELETE")

    enum struct HTTPMethod {
#define AS_HTTP_METHOD_ENUM(name, val) name,
        HTTP_METHOD_ENUM_LIST(AS_HTTP_METHOD_ENUM)
#undef AS_HTTP_METHOD_ENUM
    };

#if WIN32
    const wchar_t* toWinHttpVerb(HTTPMethod);

    struct WinHttpResponse {
        std::string body;
        DWORD status;
    };
#endif


} // namespace core

#endif // CORELIBRARY_INCLUDE_CORE_WIN_HTTP_TYPES_TYPES_HPP
