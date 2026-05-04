#ifndef CORELIBRARY_INCLUDE_CORE_HTTP_CODES_NETWORK_ERROR_HPP
#define CORELIBRARY_INCLUDE_CORE_HTTP_CODES_NETWORK_ERROR_HPP

#include <string>

#include "http_status.hpp"

#if WIN32
// clang-format off
#include <windows.h>
#include <winhttp.h>
// clang-format on

#define NETWORK_SUCCESS ERROR_SUCCESS
#define NETWORK_TIMEOUT ERROR_WINHTTP_TIMEOUT
#define NETWORK_NAME_NOT_RESOLVED ERROR_WINHTTP_NAME_NOT_RESOLVED
#define NETWORK_CANNOT_CONNECT ERROR_WINHTTP_CANNOT_CONNECT
#define NETWORK_CONNECTION_ABORTED ERROR_WINHTTP_CONNECTION_ERROR
#define NETWORK_SECURE_FAILURE ERROR_WINHTTP_SECURE_FAILURE
#define NETWORK_CLIENT_CERT_NEEDED ERROR_WINHTTP_CLIENT_AUTH_CERT_NEEDED
#define NETWORK_CERT_REVOKED ERROR_WINHTTP_SECURE_CERT_REVOKED
#define NETWORK_CERT_DATE_INVALID ERROR_WINHTTP_SECURE_CERT_DATE_INVALID
#define NETWORK_PROXY_NOT_FOUND ERROR_WINHTTP_AUTO_PROXY_SERVICE_ERROR
#define NETWORK_REDIRECT_FAILED ERROR_WINHTTP_REDIRECT_FAILED
#define NETWORK_RESEND_REQUEST ERROR_WINHTTP_RESEND_REQUEST
#define NETWORK_INVALID_RESPONSE ERROR_WINHTTP_INVALID_SERVER_RESPONSE
#define NETWORK_INVALID_URL ERROR_WINHTTP_INVALID_URL
#define NETWORK_INCORRECT_HANDLE_STATE ERROR_WINHTTP_INCORRECT_HANDLE_STATE
#define NETWORK_ACCESS_DENIED ERROR_ACCESS_DENIED
#define NETWORK_INTERNAL_ERROR ERROR_WINHTTP_INTERNAL_ERROR

#else

#define NETWORK_SUCCESS 0
#define NETWORK_TIMEOUT 1
#define NETWORK_NAME_NOT_RESOLVED 2
#define NETWORK_CANNOT_CONNECT 3
#define NETWORK_CONNECTION_ABORTED 4
#define NETWORK_SECURE_FAILURE 5
#define NETWORK_CLIENT_CERT_NEEDED 6
#define NETWORK_CERT_REVOKED 7
#define NETWORK_CERT_DATE_INVALID 8
#define NETWORK_PROXY_NOT_FOUND 9
#define NETWORK_REDIRECT_FAILED 10
#define NETWORK_RESEND_REQUEST 11
#define NETWORK_INVALID_RESPONSE 12
#define NETWORK_INVALID_URL 13
#define NETWORK_INCORRECT_HANDLE_STATE 14
#define NETWORK_ACCESS_DENIED 15
#define NETWORK_INTERNAL_ERROR 16

#endif

namespace core {

#define NETWORK_ERROR_LIST(X)                                                                                          \
    X(Success, NETWORK_SUCCESS, "Success", "The operation completed successfully.")                                      \
    X(Timeout, NETWORK_TIMEOUT, "Timeout", "The request has timed out.")                                             \
    X(NameNotResolved, NETWORK_NAME_NOT_RESOLVED, "NameNotResolved", "The server name could not be resolved.")         \
    X(CannotConnect, NETWORK_CANNOT_CONNECT, "CannotConnect", "Connection to the server failed.")                      \
    X(ConnectionAborted, NETWORK_CONNECTION_ABORTED, "ConnectionAborted", "The connection was terminated abnormally.")  \
    X(SecureFailure, NETWORK_SECURE_FAILURE, "SecureFailure", "An error occurred in the secure channel (SSL/TLS).")      \
    X(ClientCertNeeded, NETWORK_CLIENT_CERT_NEEDED, "ClientCertNeeded", "The server requires a client certificate.")     \
    X(CertRevoked, NETWORK_CERT_REVOKED, "CertRevoked", "The certificate has been revoked.")                          \
    X(CertDateInvalid, NETWORK_CERT_DATE_INVALID, "CertDateInvalid", "The certificate is expired or not yet valid.")  \
    X(ProxyNotFound, NETWORK_PROXY_NOT_FOUND, "ProxyNotFound", "The proxy server could not be located.")                \
    X(RedirectFailed, NETWORK_REDIRECT_FAILED, "RedirectFailed", "The redirect failed or changed security schemes.")   \
    X(ResendRequest, NETWORK_RESEND_REQUEST, "ResendRequest", "The request needs to be resent (e.g. for Auth).")      \
    X(InvalidResponse, NETWORK_INVALID_RESPONSE, "InvalidResponse", "The server response could not be parsed.")         \
    X(InvalidUrl, NETWORK_INVALID_URL, "InvalidUrl", "The URL scheme is invalid or unsupported.")                   \
    X(IncorrectHandleState, NETWORK_INCORRECT_HANDLE_STATE, "InvalidState", "The handle is in the wrong state for this operation.")\
    X(AccessDenied, NETWORK_ACCESS_DENIED, "AccessDenied", "Access was denied (check certificate permissions).")     \
    X(InternalError, NETWORK_INTERNAL_ERROR, "InternalError", "An internal WinHTTP error occurred.")

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

    /**
     * Check if the error is a "retryable" transient network issue for Network/Transport errors
     * @param netErr Error occurred due to system level native APIs
     * @return if "retryable", then true, else false
     */
    bool isTransient(NetworkErrorStatus netErr);

    /**
     * Check if the error is a "retryable" transient network issue for HTTP Server side failure (5xx)
     * @param status Error occurred due to system level native APIs
     * @return if "retryable", then true, else false
     */
    bool isTransient(HttpStatus status);

    int getNumericCodeOfError(NetworkErrorStatus code);

} // namespace core

#endif // CORELIBRARY_INCLUDE_CORE_HTTP_CODES_NETWORK_ERROR_HPP
