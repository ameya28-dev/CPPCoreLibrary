#include <gtest/gtest.h>
#include <core/logger/manager/manager.hpp>

TEST(LoggerTest, Initialization) {
    // Verify Init doesn't crash
    EXPECT_NO_THROW(core::LogManager::Init());
    
    EXPECT_NE(core::LogManager::Global(), nullptr);
    EXPECT_NE(core::LogManager::Print(), nullptr);
}

TEST(LoggerTest, GlobalLoggerLogging) {
    core::LogManager::Init();
    auto logger = core::LogManager::Global();
    ASSERT_NE(logger, nullptr);
    
    // Test logging at different levels
    EXPECT_NO_THROW(logger->info("Test info message"));
    EXPECT_NO_THROW(logger->warn("Test warn message"));
    EXPECT_NO_THROW(logger->error("Test error message"));
    EXPECT_NO_THROW(logger->debug("Test debug message"));
}

TEST(LoggerTest, PrintLoggerLogging) {
    core::LogManager::Init();
    auto printLogger = core::LogManager::Print();
    ASSERT_NE(printLogger, nullptr);
    
    // Test print logger
    EXPECT_NO_THROW(printLogger->info("Test print message"));
}

TEST(LoggerTest, LoggerLevels) {
    core::LogManager::Init();
    auto logger = core::LogManager::Global();
    ASSERT_NE(logger, nullptr);
    
    // Assuming logger has level control, test if levels work
    // This might depend on spdlog implementation
    EXPECT_NO_THROW(logger->set_level(spdlog::level::info));
    EXPECT_NO_THROW(logger->info("This should log"));
    EXPECT_NO_THROW(logger->debug("This might not log"));
}
