#include <gtest/gtest.h>

#include <core/utils/utils.hpp>
#include <core/win_http/handler/network.hpp>

using namespace core;

TEST(WinHttpUtilsTest, CaseInsensitiveEqual) {
    EXPECT_TRUE(isEqualCaseInsensitive("Header", "header"));
    EXPECT_TRUE(isEqualCaseInsensitive("CONTENT-TYPE", "content-type"));
    EXPECT_FALSE(isEqualCaseInsensitive("Header", "header1"));
}

TEST(WinHttpUtilsTest, ToWideString) {
    EXPECT_EQ(toWideString("test"), L"test");
    EXPECT_EQ(toWideString(""), L"");
}

TEST(WinHttpUtilsTest, UrlEncode) {
    EXPECT_EQ(urlEncode("a b"), "a%20b");
    EXPECT_EQ(urlEncode("a/b"), "a%2Fb");
    EXPECT_EQ(urlEncode("test-._~"), "test-._~");
}

TEST(WinHttpUtilsTest, UrlEncodeSpecialChars) {
    EXPECT_EQ(urlEncode("hello world!"), "hello%20world%21");
    EXPECT_EQ(urlEncode("test@example.com"), "test%40example.com");
    EXPECT_EQ(urlEncode("path/to/file"), "path%2Fto%2Ffile");
}

TEST(WinHttpUtilsTest, UrlEncodeEmptyAndUnicode) {
    EXPECT_EQ(urlEncode(""), "");
    // Assuming urlEncode handles UTF-8, but test basic
    EXPECT_EQ(urlEncode("café"), "caf%C3%A9"); // If it encodes UTF-8
}

TEST(WinHttpUtilsTest, GenerateURLEndpointEmptyParams) {
    Params params;
    std::wstring url = generateURLEndpoint(L"/path", params);
    EXPECT_EQ(url, L"/path");
}

TEST(WinHttpUtilsTest, GenerateURLEndpointSingleParam) {
    Params params    = {{"key", "value"}};
    std::wstring url = generateURLEndpoint(L"/path", params);
    EXPECT_EQ(url, L"/path?key=value");
}

TEST(WinHttpUtilsTest, CaseInsensitiveEqualEdgeCases) {
    EXPECT_TRUE(isEqualCaseInsensitive("", ""));
    EXPECT_FALSE(isEqualCaseInsensitive("a", "A "));
    EXPECT_TRUE(isEqualCaseInsensitive("ABC", "abc"));
}

#if WIN32
TEST(NetworkHandlerTest, Initialization) {
    NetworkHandler handler;
    // We can't do much without real network, but we can check if Open works
    EXPECT_NO_THROW(handler.Open(L"TestAgent"));
}
#endif

#include <core/http/base_client/base_client.hpp>
#include <core/http/codes/network_status.hpp>
#include <nlohmann/json.hpp>

TEST(HttpCodesTest, NetworkErrorStrings) {
    EXPECT_EQ(toString(NetworkErrorStatus::Timeout), "Timeout");
    EXPECT_FALSE(getDescription(NetworkErrorStatus::CannotConnect).empty());
}

TEST(HttpCodesTest, NetworkErrorNumericCodes) {
    EXPECT_EQ(getNumericCodeOfError(NetworkErrorStatus::Success), 0);
    EXPECT_NE(getNumericCodeOfError(NetworkErrorStatus::Timeout), 0);
    EXPECT_EQ(getNumericCodeOfError(NetworkErrorStatus::Unknown), -1);
}

TEST(HttpCodesTest, IsTransientNetworkErrors) {
    EXPECT_TRUE(isTransient(NetworkErrorStatus::Timeout));
    EXPECT_TRUE(isTransient(NetworkErrorStatus::CannotConnect));
    EXPECT_TRUE(isTransient(NetworkErrorStatus::ConnectionAborted));
    EXPECT_TRUE(isTransient(NetworkErrorStatus::ResendRequest));
    EXPECT_FALSE(isTransient(NetworkErrorStatus::Success));
    EXPECT_FALSE(isTransient(NetworkErrorStatus::AccessDenied));
}

TEST(HttpCodesTest, IsTransientHttpStatus) {
    EXPECT_TRUE(isTransient(HttpStatus::InternalServerError));
    EXPECT_TRUE(isTransient(HttpStatus::BadGateway));
    EXPECT_FALSE(isTransient(HttpStatus::Ok));
    EXPECT_FALSE(isTransient(HttpStatus::NotFound));
}

TEST(HttpCodesTest, AllNetworkErrorEnums) {
    // Ensure all enum values are covered
    EXPECT_EQ(toString(NetworkErrorStatus::Success), "Success");
    EXPECT_EQ(toString(NetworkErrorStatus::Timeout), "Timeout");
    EXPECT_EQ(toString(NetworkErrorStatus::NameNotResolved), "NameNotResolved");
    EXPECT_EQ(toString(NetworkErrorStatus::CannotConnect), "CannotConnect");
    EXPECT_EQ(toString(NetworkErrorStatus::ConnectionAborted), "ConnectionAborted");
    EXPECT_EQ(toString(NetworkErrorStatus::SecureFailure), "SecureFailure");
    EXPECT_EQ(toString(NetworkErrorStatus::ClientCertNeeded), "ClientCertNeeded");
    EXPECT_EQ(toString(NetworkErrorStatus::CertRevoked), "CertRevoked");
    EXPECT_EQ(toString(NetworkErrorStatus::CertDateInvalid), "CertDateInvalid");
    EXPECT_EQ(toString(NetworkErrorStatus::ProxyNotFound), "ProxyNotFound");
    EXPECT_EQ(toString(NetworkErrorStatus::RedirectFailed), "RedirectFailed");
    EXPECT_EQ(toString(NetworkErrorStatus::ResendRequest), "ResendRequest");
    EXPECT_EQ(toString(NetworkErrorStatus::InvalidResponse), "InvalidResponse");
    EXPECT_EQ(toString(NetworkErrorStatus::InvalidUrl), "InvalidUrl");
    EXPECT_EQ(toString(NetworkErrorStatus::IncorrectHandleState), "InvalidState");
    EXPECT_EQ(toString(NetworkErrorStatus::AccessDenied), "AccessDenied");
    EXPECT_EQ(toString(NetworkErrorStatus::InternalError), "InternalError");
    EXPECT_EQ(toString(NetworkErrorStatus::Unknown), "Unknown");
}

TEST(HttpCodesTest, NetworkErrorDescriptions) {
    // Ensure descriptions are not empty
    EXPECT_FALSE(getDescription(NetworkErrorStatus::Success).empty());
    EXPECT_FALSE(getDescription(NetworkErrorStatus::Timeout).empty());
    EXPECT_FALSE(getDescription(NetworkErrorStatus::Unknown).empty());
}

#if WIN32
TEST(NetworkHandlerTest, ToNetworkErrorStatusMapping) {
    // Test the mapping function
    EXPECT_EQ(toNetworkErrorStatus(ERROR_SUCCESS), NetworkErrorStatus::Success);
    EXPECT_EQ(toNetworkErrorStatus(ERROR_WINHTTP_TIMEOUT), NetworkErrorStatus::Timeout);
    EXPECT_EQ(toNetworkErrorStatus(999999), NetworkErrorStatus::Unknown); // Invalid code
}
#endif

TEST(HttpCodesTest, IsTransient) {
    // Test transient network errors
    EXPECT_TRUE(isTransient(NetworkErrorStatus::Timeout));
    EXPECT_TRUE(isTransient(NetworkErrorStatus::CannotConnect));
    EXPECT_TRUE(isTransient(NetworkErrorStatus::ConnectionAborted));
    EXPECT_TRUE(isTransient(NetworkErrorStatus::ResendRequest));

    // Test transient HTTP errors (5xx)
    EXPECT_TRUE(isTransient(HttpStatus::InternalServerError));
    EXPECT_TRUE(isTransient(HttpStatus::BadGateway));

    // Test non-transient errors
    EXPECT_FALSE(isTransient(NetworkErrorStatus::InvalidUrl));
    EXPECT_FALSE(isTransient(NetworkErrorStatus::Success));
    EXPECT_FALSE(isTransient(HttpStatus::Ok));
    EXPECT_FALSE(isTransient(HttpStatus::BadRequest));
}

struct MockData {
    int id;
    std::string name;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(MockData, id, name)

TEST(HttpTypesTest, JsonTraits) {
    EXPECT_TRUE(IsJsonDeserializable<MockData>::value);
    EXPECT_TRUE(IsJsonSerializable<MockData>::value);
    EXPECT_TRUE(IsJsonDeserializable<int>::value);
}
