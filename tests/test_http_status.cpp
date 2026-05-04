#include <gtest/gtest.h>

#include <core/http/codes/http_status.hpp>

using namespace core;

TEST(HttpStatusTest, EnumValues) {
    EXPECT_EQ(static_cast<int>(HttpStatus::Continue), 100);
    EXPECT_EQ(static_cast<int>(HttpStatus::Ok), 200);
    EXPECT_EQ(static_cast<int>(HttpStatus::Created), 201);
    EXPECT_EQ(static_cast<int>(HttpStatus::Accepted), 202);
    EXPECT_EQ(static_cast<int>(HttpStatus::NoContent), 204);
    EXPECT_EQ(static_cast<int>(HttpStatus::MovedPermanently), 301);
    EXPECT_EQ(static_cast<int>(HttpStatus::Found), 302);
    EXPECT_EQ(static_cast<int>(HttpStatus::NotModified), 304);
    EXPECT_EQ(static_cast<int>(HttpStatus::BadRequest), 400);
    EXPECT_EQ(static_cast<int>(HttpStatus::Unauthorized), 401);
    EXPECT_EQ(static_cast<int>(HttpStatus::Forbidden), 403);
    EXPECT_EQ(static_cast<int>(HttpStatus::NotFound), 404);
    EXPECT_EQ(static_cast<int>(HttpStatus::MethodNotAllowed), 405);
    EXPECT_EQ(static_cast<int>(HttpStatus::InternalServerError), 500);
    EXPECT_EQ(static_cast<int>(HttpStatus::NotImplemented), 501);
    EXPECT_EQ(static_cast<int>(HttpStatus::BadGateway), 502);
    EXPECT_EQ(static_cast<int>(HttpStatus::ServiceUnavailable), 503);
}

TEST(HttpStatusTest, ToString) {
    EXPECT_STREQ(toString(HttpStatus::Ok), "Ok");
    EXPECT_STREQ(toString(HttpStatus::NotFound), "NotFound");
    EXPECT_STREQ(toString(HttpStatus::InternalServerError), "InternalServerError");
}

TEST(HttpStatusTest, GetDescription) {
    EXPECT_STREQ(getDescription(HttpStatus::Ok), "Request Successful");
    EXPECT_STREQ(getDescription(HttpStatus::BadRequest), "Invalid Request Parameter");
}
