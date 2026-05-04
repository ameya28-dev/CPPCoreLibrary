#include <gtest/gtest.h>
#include <core/logger/manager/manager.hpp>

TEST(LoggerTest, Initialization) {
    // Verify Init doesn't crash
    EXPECT_NO_THROW(core::LogManager::Init());
    
    EXPECT_NE(core::LogManager::Global(), nullptr);
    EXPECT_NE(core::LogManager::Print(), nullptr);
}
