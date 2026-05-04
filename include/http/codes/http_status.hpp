#ifndef CORELIBRARY_INCLUDE_HTTP_CODES_HTTP_STATUS_HPP
#define CORELIBRARY_INCLUDE_HTTP_CODES_HTTP_STATUS_HPP

#include <array>

namespace core {

#define HTTP_STATUS_LIST(X)                                               \
    X(Continue, = 100, "Continue With Request")                           \
    X(SwitchingProtocols, = 101, "Switching To New Protocol")             \
    X(Processing, = 102, "Processing Request")                            \
    X(Ok, = 200, "Request Successful")                                    \
    X(Created, = 201, "Resource Created")                                 \
    X(Accepted, = 202, "Request Accepted For Processing")                 \
    X(NonAuthoritativeInformation, = 203, "Non-Authoritative Info")       \
    X(NoContent, = 204, "Success - No Content Returned")                  \
    X(ResetContent, = 205, "Reset View")                                  \
    X(PartialContent, = 206, "Partial Content Delivered")                 \
    X(MultiStatus, = 207, "Multi-Status Response")                        \
    X(AlreadyReported, = 208, "Already Reported In Multi-Status")         \
    X(ImUsed, = 226, "Instance Manipulations Applied")                    \
    X(MultipleChoices, = 300, "Multiple Options Available")               \
    X(MovedPermanently, = 301, "Resource Moved Permanently")              \
    X(Found, = 302, "Resource Found At Different URI")                    \
    X(SeeOther, = 303, "See Other URI")                                   \
    X(NotModified, = 304, "Resource Not Modified")                        \
    X(UseProxy, = 305, "Use Proxy Specified In Location")                 \
    X(TemporaryRedirect, = 307, "Temporary Redirect")                     \
    X(PermanentRedirect, = 308, "Permanent Redirect")                     \
    X(BadRequest, = 400, "Invalid Request Parameter")                     \
    X(Unauthorized, = 401, "Authentication Required")                     \
    X(PaymentRequired, = 402, "Payment Required")                         \
    X(Forbidden, = 403, "Access Forbidden")                               \
    X(NotFound, = 404, "Resource Not Found")                              \
    X(MethodNotAllowed, = 405, "HTTP Method Not Allowed")                 \
    X(NotAcceptable, = 406, "Content Not Acceptable")                     \
    X(ProxyAuthenticationRequired, = 407, "Proxy Auth Required")          \
    X(RequestTimeout, = 408, "Request Timed Out")                         \
    X(Conflict, = 409, "Resource Conflict")                               \
    X(Gone, = 410, "Resource Permanently Removed")                        \
    X(LengthRequired, = 411, "Content-Length Header Required")            \
    X(PreconditionFailed, = 412, "Precondition Failed")                   \
    X(RequestEntityTooLarge, = 413, "Request Payload Too Large")          \
    X(RequestUriTooLong, = 414, "Request URI Too Long")                   \
    X(UnsupportedMediaType, = 415, "Unsupported Media Type")              \
    X(RequestedRangeNotSatisfiable, = 416, "Requested Range Invalid")     \
    X(ExpectationFailed, = 417, "Expectation Header Failed")              \
    X(MisdirectedRequest, = 421, "Misdirected Request")                   \
    X(UnprocessableEntity, = 422, "Semantic Errors In Request")           \
    X(Locked, = 423, "Resource Is Locked")                                \
    X(FailedDependency, = 424, "Failed Dependency")                       \
    X(UpgradeRequired, = 426, "Upgrade To TLS/Other Required")            \
    X(PreconditionRequired, = 428, "Precondition Required")               \
    X(TooManyRequests, = 429, "Rate Limit Exceeded")                      \
    X(RequestHeaderFieldsTooLarge, = 431, "Request Headers Too Large")    \
    X(ConnectionClosedWithoutResponse, = 444, "No Response From Server")  \
    X(UnavailableForLegalReasons, = 451, "Unavailable For Legal Reasons") \
    X(ClientClosedRequest, = 499, "Client Closed Connection")             \
    X(InternalServerError, = 500, "Internal Server Error")                \
    X(NotImplemented, = 501, "Feature Not Implemented")                   \
    X(BadGateway, = 502, "Invalid Response From Upstream")                \
    X(ServiceUnavailable, = 503, "Service Temporarily Overloaded")        \
    X(GatewayTimeout, = 504, "Upstream Gateway Timed Out")                \
    X(HttpVersionNotSupported, = 505, "HTTP Version Not Supported")       \
    X(VariantAlsoNegotiates, = 506, "Content Negotiation Error")          \
    X(InsufficientStorage, = 507, "Insufficient Server Storage")          \
    X(LoopDetected, = 508, "Infinite Loop Detected")                      \
    X(NotExtended, = 510, "Further Extensions Required")                  \
    X(NetworkAuthenticationRequired, = 511, "Network Auth Required")

    enum struct HttpStatus {
#define AS_HTTP_STATUS_CODES(name, value, desc) name value,
        HTTP_STATUS_LIST(AS_HTTP_STATUS_CODES)
#undef AS_HTTP_STATUS_CODES
    };

    static constexpr HttpStatus httpStatusList[] = {
#define AS_HTTP_STATUS_LIST(name, value, desc) HttpStatus::name,
        HTTP_STATUS_LIST(AS_HTTP_STATUS_LIST)
#undef AS_HTTP_STATUS_LIST
    };

    const char* toString(HttpStatus status);

    const char* getDescription(HttpStatus status);

    static constexpr std::size_t HttpStatusCount = std::size(httpStatusList);

    constexpr std::array<HttpStatus, HttpStatusCount> httpStatusArray = {{
#define AS_HTTP_STATUS_LIST(name, value, desc) HttpStatus::name,
        HTTP_STATUS_LIST(AS_HTTP_STATUS_LIST)
#undef AS_HTTP_STATUS_LIST
    }};

    std::array<HttpStatus, HttpStatusCount> getHttpStatusList();

} // namespace core

#endif // CORELIBRARY_INCLUDE_HTTP_CODES_HTTP_STATUS_HPP
