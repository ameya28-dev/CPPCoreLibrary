#include <chrono>
#include <gtest/gtest.h>
#include <string>
#include <variant>

#include <core/http/base_client/base_client.hpp>
#include <core/http/codes/http_status.hpp>
#include <core/http/codes/network_status.hpp>
#include <core/http/types/types.hpp>
#include <nlohmann/json.hpp>

using namespace core;

class BaseClientTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Any setup if needed
    }

    void TearDown() override {
        // Cleanup
    }
};

TEST_F(BaseClientTest, ConstructionAndConfiguration) {
    BaseClient client("TestAgent", "example.com", false);
    EXPECT_NO_THROW(client.SetVerbose(true));
    EXPECT_NO_THROW(client.SetRetries(2));
    EXPECT_NO_THROW(client.SetTimeouts(std::chrono::milliseconds(100), std::chrono::milliseconds(100),
        std::chrono::milliseconds(100), std::chrono::milliseconds(100)));
}

#ifndef WIN32
TEST_F(BaseClientTest, NonWindowsFallbackResult) {
    BaseClient client("TestAgent", "example.com", false);
    auto result = client.Get<std::string>("http://example.com");

    EXPECT_TRUE(std::holds_alternative<Failure<>>(result));
    const auto* failure = std::get_if<Failure<>>(&result);
    ASSERT_NE(failure, nullptr);
    EXPECT_NE(failure->message, "");
    EXPECT_EQ(failure->rawStatus, -1);
}
#endif

TEST_F(BaseClientTest, NetworkResultStructure) {
    NetworkResult result;
    result.body   = "OK";
    result.status = HttpStatus::Ok;

    EXPECT_EQ(result.body, "OK");
    EXPECT_EQ(result.status, HttpStatus::Ok);
}

TEST_F(BaseClientTest, Construction) {
    // Construction should succeed even without network, as it just opens session/connect handles
    BaseClient client("TestAgent", "localhost", false);
    // Note: If localhost isn't resolvable or something, it might fail,
    // but usually WinHttpConnect doesn't validate much until Request.
}

// Since BaseClient is hard to test without a real server or mocking NetworkHandler,
// we focus on what we can.
