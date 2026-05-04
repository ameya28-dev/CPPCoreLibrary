#ifndef CORELIBRARY_INCLUDE_HTTP_CODES_NETWORK_ERROR_HPP
#define CORELIBRARY_INCLUDE_HTTP_CODES_NETWORK_ERROR_HPP

#include <optional>
#include <string>

#include "http_status.hpp"

#if WIN32
// clang-format off
#include <windows.h>
#include <winhttp.h>
// clang-format on
#endif

namespace core {

#define NETWORK_ERROR_LIST(X)                                                                                          \
    X(Success, ERROR_SUCCESS, "Success", "The operation completed successfully.")                                      \
    X(Timeout, ERROR_WINHTTP_TIMEOUT, "Timeout", "The request has timed out.")                                         \
    X(NameNotResolved, ERROR_WINHTTP_NAME_NOT_RESOLVED, "NameNotResolved", "The server name could not be resolved.")   \
    X(CannotConnect, ERROR_WINHTTP_CANNOT_CONNECT, "CannotConnect", "Connection to the server failed.")                \
    X(ConnectionAborted, ERROR_WINHTTP_CONNECTION_ERROR, "ConnectionAborted",                                          \
        "The connection was terminated abnormally.")                                                                   \
    X(SecureFailure, ERROR_WINHTTP_SECURE_FAILURE, "SecureFailure",                                                    \
        "An error occurred in the secure channel (SSL/TLS).")                                                          \
    X(ClientCertNeeded, ERROR_WINHTTP_CLIENT_AUTH_CERT_NEEDED, "ClientCertNeeded",                                     \
        "The server requires a client certificate.")                                                                   \
    X(CertRevoked, ERROR_WINHTTP_SECURE_CERT_REVOKED, "CertRevoked", "The certificate has been revoked.")              \
    X(CertDateInvalid, ERROR_WINHTTP_SECURE_CERT_DATE_INVALID, "CertDateInvalid",                                      \
        "The certificate is expired or not yet valid.")                                                                \
    X(ProxyNotFound, ERROR_WINHTTP_AUTO_PROXY_SERVICE_ERROR, "ProxyNotFound",                                          \
        "The proxy server could not be located.")                                                                      \
    X(RedirectFailed, ERROR_WINHTTP_REDIRECT_FAILED, "RedirectFailed",                                                 \
        "The redirect failed or changed security schemes.")                                                            \
    X(ResendRequest, ERROR_WINHTTP_RESEND_REQUEST, "ResendRequest", "The request needs to be resent (e.g. for Auth).") \
    X(InvalidResponse, ERROR_WINHTTP_INVALID_SERVER_RESPONSE, "InvalidResponse",                                       \
        "The server response could not be parsed.")                                                                    \
    X(InvalidUrl, ERROR_WINHTTP_INVALID_URL, "InvalidUrl", "The URL scheme is invalid or unsupported.")                \
    X(IncorrectHandleState, ERROR_WINHTTP_INCORRECT_HANDLE_STATE, "InvalidState",                                      \
        "The handle is in the wrong state for this operation.")                                                        \
    X(AccessDenied, ERROR_ACCESS_DENIED, "AccessDenied", "Access was denied (check certificate permissions).")         \
    X(InternalError, ERROR_WINHTTP_INTERNAL_ERROR, "InternalError", "An internal WinHTTP error occurred.")

    // Define the Enum
    enum struct NetworkErrorStatus {
#define X(name, code, str, desc) name,
        NETWORK_ERROR_LIST(X)
#undef X
            Unknown
    };

    // Map DWORD to Enum
#if WIN32
    NetworkErrorStatus toNetworkErrorStatus(DWORD error);
#endif

    // Get Short Name (e.g., "Timeout")
    std::string toString(NetworkErrorStatus code);

    // Get Detailed Description
    std::string getDescription(NetworkErrorStatus code);

    // Helper: Check if the error is a "retryable" transient network issue
    bool isTransient(std::optional<NetworkErrorStatus> code, std::optional<HttpStatus> status);

    int getNumericCodeOfError(NetworkErrorStatus code);

} // namespace core

#endif // CORELIBRARY_INCLUDE_HTTP_CODES_NETWORK_ERROR_HPP
