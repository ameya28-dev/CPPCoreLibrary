#include "core/http/base_client/base_client.hpp"

core::BaseClient::BaseClient(const std::string& agent, const std::string& host, const bool isHTTPS) {
#if WIN32
    try {
        const auto wAgent = toWideString(agent);
        _handler.Open(wAgent);
        const auto wHost = toWideString(host);
        _handler.Connect(wHost, isHTTPS);
        _isOKConstruct = true;
    } catch (const std::system_error& ex) {
        LOG_FATAL("{}: Failed to construct a WinHttp Network handle: {}", ex.code().value(), ex.what());
        _isOKConstruct = false;
    }
#endif
}

auto core::BaseClient::IsConstructed() const -> bool {
    return _isOKConstruct;
}

auto core::BaseClient::SetVerbose(const bool v) -> void {
    _verbose = v;
}

auto core::BaseClient::SetRetries(const int r) -> void {
    _retryCount = r;
}

auto core::BaseClient::SetTimeouts(const std::chrono::milliseconds connect, const std::chrono::milliseconds send,
    const std::chrono::milliseconds receive, const std::chrono::milliseconds resolve) const -> void {
#if WIN32
    _handler.SetTimeouts(connect, send, receive, resolve);
#endif
}

auto core::BaseClient::Delete(const std::string& path, const Params& params, const Headers& headers)
    -> ApiResult<Empty> {
    return _executeWithRetry<Empty>(
        [&](const std::string& p) { return _sendRequest(HTTPMethod::Delete, p, params, headers); }, path);
}

core::NetworkResponse core::BaseClient::_sendRequest(
    const HTTPMethod method, const std::string& path, const Params& params, const Headers& headers) {
#if WIN32
    try {
        const auto wPath = toWideString(path);
        const auto url   = generateURLEndpoint(wPath, params);
        _handler.Request(method, url.c_str());
        _handler.AddHeaders(headers);
        _handler.SendRequest();
        _handler.ReceiveResponse();
        const auto [body, status] = _handler.ReadResponse();
        return NetworkResult{body, static_cast<HttpStatus>(status)};
    } catch (const std::system_error& ex) {
        return NetworkError{ex.what(), toNetworkErrorStatus(ex.code().value())};
    } catch (const std::runtime_error& ex) {
        return NetworkError{ex.what(), NetworkErrorStatus::InternalError};
    }
#endif
}
