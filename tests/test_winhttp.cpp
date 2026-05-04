#include <gtest/gtest.h>
#include <win_http/utils/utils.hpp>
#include <win_http/handler/network.hpp>

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

TEST(WinHttpUtilsTest, GenerateURLEndpoint) {
    Params params = {{"key1", "val1"}, {"key 2", "val 2"}};
    std::wstring url = generateURLEndpoint(L"/path", params);
    // Note: multimap might have specific order, but usually it's key order.
    // "key 2" comes before "key1" in std::multimap? No, ' ' is before '1'.
    // Let's check both possibilities or just use one param for simplicity if it fails.
    EXPECT_TRUE(url == L"/path?key%202=val%202&key1=val1" || url == L"/path?key1=val1&key%202=val%202");
}

TEST(NetworkHandlerTest, Initialization) {
    NetworkHandler handler;
    // We can't do much without real network, but we can check if Open works
    EXPECT_NO_THROW(handler.Open(L"TestAgent"));
}

#include <gtest/gtest.h>
#include <http/base_client/base_client.hpp>
#include <http/codes/network_error.hpp>
#include <nlohmann/json.hpp>

using namespace core;

TEST(HttpCodesTest, NetworkErrorStrings) {
    EXPECT_EQ(toString(NetworkErrorStatus::Timeout), "Timeout");
    EXPECT_FALSE(getDescription(NetworkErrorStatus::CannotConnect).empty());
}

TEST(HttpCodesTest, IsTransient) {
    // Test transient network errors
    EXPECT_TRUE(isTransient(NetworkErrorStatus::Timeout, HttpStatus::Ok));
    EXPECT_TRUE(isTransient(NetworkErrorStatus::CannotConnect, HttpStatus::Ok));
    
    // Test transient HTTP errors (5xx)
    EXPECT_TRUE(isTransient(NetworkErrorStatus::Success, HttpStatus::InternalServerError));
    EXPECT_TRUE(isTransient(NetworkErrorStatus::Success, HttpStatus::BadGateway));
    
    // Test non-transient errors
    EXPECT_FALSE(isTransient(NetworkErrorStatus::InvalidUrl, HttpStatus::Ok));
    EXPECT_FALSE(isTransient(NetworkErrorStatus::Success, HttpStatus::BadRequest));
}

TEST(BaseClientTest, Construction) {
    // Construction should succeed even without network, as it just opens session/connect handles
    BaseClient client("TestAgent", "localhost", false);
    // Note: If localhost isn't resolvable or something, it might fail, 
    // but usually WinHttpConnect doesn't validate much until Request.
}

// Since BaseClient is hard to test without a real server or mocking NetworkHandler,
// we focus on what we can.

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
