#include "core/win_http/handler/network.hpp"

#if WIN32

#include <cassert>
#include <system_error>

#include "core/win_http/constants/constants.hpp"

void core::NetworkHandler::Open(const std::wstring& agent) {
    _hSession.reset(WinHttpOpen(
        agent.c_str(), WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0));

    if (!_hSession) {
        const DWORD error = GetLastError();
        printError("Session Init", error);
        throw std::system_error(
            static_cast<int>(error), std::system_category(), "Failed to initialize WinHttp Session");
    }

    using namespace constants;
    SetTimeouts(Timeouts::Connect, Timeouts::Send, Timeouts::Receive);

    _state = State::session;
}

void core::NetworkHandler::SetTimeouts(const std::chrono::milliseconds connect, const std::chrono::milliseconds send,
    const std::chrono::milliseconds receive, const std::chrono::milliseconds resolve) const {
    if (!WinHttpSetTimeouts(_hSession.get(), static_cast<int>(resolve.count()), static_cast<int>(connect.count()),
            static_cast<int>(send.count()), static_cast<int>(receive.count()))) {
        DWORD error = GetLastError();
        printError("Set Timeouts", error);
        if (_isStrict) {
            throw std::system_error(
                static_cast<int>(error), std::system_category(), "Failed to initialize WinHttp Connect");
        }
    }
}

void core::NetworkHandler::Connect(const std::wstring& host, const bool isHTTPS) {
    _assertStates(_state >= State::session, "Trying to connect before initializing WinHttp Session");

    _isSecure = isHTTPS;
    _hConnect.reset(WinHttpConnect(
        _hSession.get(), host.c_str(), isHTTPS ? INTERNET_DEFAULT_HTTPS_PORT : INTERNET_DEFAULT_HTTP_PORT, 0));

    if (!_hConnect) {
        const DWORD error = GetLastError();
        printError("Connect", error);
        throw std::system_error(
            static_cast<int>(error), std::system_category(), "Failed to initialize WinHttp Connect");
    }

    _state = State::connect;
}

void core::NetworkHandler::Request(const HTTPMethod method, const wchar_t* url) {
    _assertStates(_state >= State::connect, "Trying to request before initializing WinHttp Connect");

    const DWORD requestFlags = _isSecure ? WINHTTP_FLAG_SECURE : 0;
    _hRequest.reset(WinHttpOpenRequest(_hConnect.get(), toWinHttpVerb(method), url, nullptr, WINHTTP_NO_REFERER,
        WINHTTP_DEFAULT_ACCEPT_TYPES, requestFlags));
    if (!_hRequest) {
        _state            = State::connect;
        const DWORD error = GetLastError();
        printError("Request", error);
        throw std::system_error(
            static_cast<int>(error), std::system_category(), "Failed to initialize WinHttp Request");
    }

    _state = State::request;
}

void core::NetworkHandler::AddHeaders(const Headers& headers) {
    _assertStates(_state >= State::request, "Trying to add headers to a request not initialized yet");
    _assertStates(_state < State::send, "Trying to add headers after request is sent");
    std::wstring headerLine{};
    for (const auto& [key, value] : headers) {
        headerLine += toWideString(key) + L": " + toWideString(value) + L"\r\n";
    }
    if (headerLine.empty()) {
        return;
    }
    if (!WinHttpAddRequestHeaders(_hRequest.get(), headerLine.c_str(), -1L, WINHTTP_ADDREQ_FLAG_ADD)) {
        _state            = State::connect;
        const DWORD error = GetLastError();
        printError("Request", error);
        throw std::system_error(
            static_cast<int>(error), std::system_category(), "Failed to add Headers to the Request");
    }
}

void core::NetworkHandler::SendRequest() {
    _send(nullptr, 0);
}

void core::NetworkHandler::SendRequestWithBody(const std::string& body) {
    _send(body.c_str(), static_cast<DWORD>(body.length()));
}

void core::NetworkHandler::ReceiveResponse() {
    _assertStates(_state >= State::send, "Trying to receive response before sending request");

    if (!WinHttpReceiveResponse(_hRequest.get(), nullptr)) {
        _state            = State::connect;
        const DWORD error = GetLastError();
        printError("Receive Response", error);
        throw std::system_error(static_cast<int>(error), std::system_category(), "Failed to receive response!");
    }

    _state = State::receive;
}

uint32_t core::NetworkHandler::GetStatusCode() {
    _assertStates(_state >= State::receive, "Trying to parse response before receiving it");

    DWORD dwStatusCode = 0, dwSize = sizeof(dwStatusCode);

    if (!WinHttpQueryHeaders(_hRequest.get(), WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
            WINHTTP_HEADER_NAME_BY_INDEX, &dwStatusCode, &dwSize, WINHTTP_NO_HEADER_INDEX)) {
        _state            = State::connect;
        const DWORD error = GetLastError();
        printError("Response Headers", error);
        throw std::system_error(
            static_cast<int>(error), std::system_category(), "Failed to get status code of the response");
    }

    return dwStatusCode;
}

core::WinHttpResponse core::NetworkHandler::ReadResponse() {
    _assertStates(_state >= State::receive, "Trying to parse response before receiving it");
    std::string response;
    DWORD bytesRead          = 0;
    constexpr int bufferSize = constants::EightKBSize;
    std::vector<char> readBuffer(bufferSize);

    while (true) {
        DWORD bytesAvailable = 0;
        if (!WinHttpQueryDataAvailable(_hRequest.get(), &bytesAvailable)) {
            _state            = State::connect;
            const DWORD error = GetLastError();
            printError("Query Data Available", error);
            throw std::system_error(
                static_cast<int>(error), std::system_category(), "WinHttpQueryDataAvailable failed");
        }

        if (bytesAvailable == 0) {
            break;
        }

        if (!WinHttpReadData(_hRequest.get(), readBuffer.data(), bufferSize, &bytesRead)) {
            _state            = State::connect;
            const DWORD error = GetLastError();
            printError("Read Data", error);
            throw std::system_error(static_cast<int>(error), std::system_category(), "WinHttpReadData failed");
        }

        if (bytesRead > 0) {
            response.append(readBuffer.data(), bytesRead);
        } else {
            break;
        }
    }
    const auto statusCode = GetStatusCode();
    _state                = State::connect;
    return WinHttpResponse{response, statusCode};
}

void core::NetworkHandler::SetIsStrict(const bool isStrict) {
    _isStrict = isStrict;
}

void core::NetworkHandler::_assertStates(const bool condition, const char* message) {
#if NDEBUG
    if (!condition) {
        throw std::logic_error(message);
    }
#else
    assert(condition && message);
#endif
}

void core::NetworkHandler::_send(const LPCVOID data, const DWORD size) {
    _assertStates(_state >= State::request, "Trying to send request before initializing WinHttp Request");

    // WinHTTP allows up to 5-10 retries for authentication handshakes
    constexpr int maxImmediateRetries = 5;

    for (int attempt = 0; attempt < maxImmediateRetries; ++attempt) {
        if (WinHttpSendRequest(_hRequest.get(), WINHTTP_NO_ADDITIONAL_HEADERS, 0, (LPVOID) data, size, size, 0)) {
            _state = State::send;
            return; // Success!
        }

        const DWORD error = GetLastError();

        // If it's specifically a RESEND request, don't throw, just loop immediately
        if (error == ERROR_WINHTTP_RESEND_REQUEST) {
            continue;
        }

        // For any other error, update state and throw as before
        _state = State::connect;
        printError("Send Request", error);
        throw std::system_error(static_cast<int>(error), std::system_category(), "Failed to send request!");
    }

    // If we exit the loop, we hit the max immediate retries for RESEND
    throw std::runtime_error("Max immediate WinHTTP resend attempts reached.");
}

#endif
