#ifndef CORELIBRARY_INCLUDE_CORE_WIN_HTTP_HANDLER_NETWORK_HPP
#define CORELIBRARY_INCLUDE_CORE_WIN_HTTP_HANDLER_NETWORK_HPP

#include <chrono>
#include <string>

#include "core/utils/utils.hpp"
#include "core/win_http/types/types.hpp"

namespace core {

#if WIN32
    class NetworkHandler {
    public:
        NetworkHandler() = default;

        void Open(const std::wstring& agent);

        void SetTimeouts(std::chrono::milliseconds connect, std::chrono::milliseconds send,
            std::chrono::milliseconds receive, std::chrono::milliseconds resolve = std::chrono::milliseconds{0}) const;

        void Connect(const std::wstring& host, bool isHTTPS);

        void Request(HTTPMethod method, const wchar_t* url);

        void AddHeaders(const Headers& headers);

        void SendRequest();

        void SendRequestWithBody(const std::string& body);

        void ReceiveResponse();

        [[nodiscard]] uint32_t GetStatusCode();

        [[nodiscard]] WinHttpResponse ReadResponse();

        void SetIsStrict(bool isStrict);

    private:
        enum class State : int { none = 0, session, connect, request, send, receive };

    private:
        static void _assertStates(bool, const char*);

        void _send(LPCVOID, DWORD);

    private:
        ScopedHInternet _hSession, _hConnect, _hRequest;
        bool _isStrict = true;
        bool _isSecure = false;
        State _state   = State::none;
    };
#endif

} // namespace core

#endif // CORELIBRARY_INCLUDE_CORE_WIN_HTTP_HANDLER_NETWORK_HPP
