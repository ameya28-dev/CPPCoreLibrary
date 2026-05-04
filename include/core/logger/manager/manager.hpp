#ifndef CORELIBRARY_INCLUDE_CORE_LOGGER_MANAGER_MANAGER_HPP
#define CORELIBRARY_INCLUDE_CORE_LOGGER_MANAGER_MANAGER_HPP

#include <memory>
#include <spdlog/logger.h>
#include <string_view>

#define LOG_INIT() core::LogManager::Init()

#ifndef NDEBUG

#define LOG_TRACE(...)   core::LogManager::Global()->trace(__VA_ARGS__)
#define LOG_DEBUG(...)   core::LogManager::Global()->debug(__VA_ARGS__)
#define LOG_INFO(...)    core::LogManager::Global()->info(__VA_ARGS__)
#define LOG_WARNING(...) core::LogManager::Global()->warn(__VA_ARGS__)

#define PRINT_TRACE(...)   core::LogManager::Print()->trace(__VA_ARGS__)
#define PRINT_DEBUG(...)   core::LogManager::Print()->debug(__VA_ARGS__)
#define PRINT_INFO(...)    core::LogManager::Print()->info(__VA_ARGS__)
#define PRINT_WARNING(...) core::LogManager::Print()->warn(__VA_ARGS__)

#else

#define LOG_TRACE(...)
#define LOG_DEBUG(...)
#define LOG_INFO(...)
#define LOG_WARNING(...)

#define PRINT_TRACE(...)
#define PRINT_DEBUG(...)
#define PRINT_INFO(...)
#define PRINT_WARNING(...)

#endif

#define LOG_ERROR(...) core::LogManager::Global()->error(__VA_ARGS__)
#define LOG_FATAL(...) core::LogManager::ReportCoreFatal(__FILE__, __LINE__, fmt::format(__VA_ARGS__))

#define PRINT_ERROR(...) core::LogManager::Print()->error(__VA_ARGS__)
#define PRINT_FATAL(...) core::LogManager::ReportPrintFatal(__FILE__, __LINE__, fmt::format(__VA_ARGS__))

namespace core {
    class LogManager {
    public:
        static void Init();

        static std::shared_ptr<spdlog::logger>& Global();

        static std::shared_ptr<spdlog::logger>& Print();

        static void InjectSpacer();

        static void ReportCoreFatal(const char* file, int line, std::string_view message);

        static void ReportPrintFatal(const char* file, int line, std::string_view message);

    private:
        static std::shared_ptr<spdlog::logger> _globalLogger, _printLogger;
    };

    inline void testPrintLogs() {
        PRINT_TRACE("PRINTING TRACE MESSAGE");
        PRINT_DEBUG("PRINTING DEBUG MESSAGE");
        PRINT_INFO("PRINTING INFO  MESSAGE");
        PRINT_WARNING("PRINTING WARNING MESSAGE");
        PRINT_ERROR("PRINTING ERROR MESSAGE");
        PRINT_FATAL("PRINTING FATAL MESSAGE");

        PRINT_TRACE("printing trace message");
        PRINT_DEBUG("printing debug message");
        PRINT_INFO("printing info  message");
        PRINT_WARNING("printing warning message");
        PRINT_ERROR("printing error message");
        PRINT_FATAL("printing fatal message");

        LOG_TRACE("LOGGING TRACE MESSAGE");
        LOG_DEBUG("LOGGING DEBUG MESSAGE");
        LOG_INFO("LOGGING INFO  MESSAGE");
        LOG_WARNING("LOGGING WARNING MESSAGE");
        LOG_ERROR("LOGGING ERROR MESSAGE");
        LOG_FATAL("LOGGING FATAL MESSAGE");

        LOG_TRACE("logging trace message");
        LOG_DEBUG("logging debug message");
        LOG_INFO("logging info  message");
        LOG_WARNING("logging warning message");
        LOG_ERROR("logging error message");
        LOG_FATAL("logging fatal message");
    }

} // namespace core

#endif // CORELIBRARY_INCLUDE_CORE_LOGGER_MANAGER_MANAGER_HPP
