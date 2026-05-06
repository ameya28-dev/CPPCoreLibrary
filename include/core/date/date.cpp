#include "date.hpp"

#if WIN32
#include <filesystem>

#include <windows.h>
#endif

void core::setTimeZoneDB() {
#if WIN32
    char buffer[MAX_PATH];
    GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    std::filesystem::path exePath(buffer);
    // Look for /tzdata in the same directory as the .exe
    std::filesystem::path tzPath = exePath.parent_path() / "tzdata";

    date::set_install(tzPath.string());
    try {
        date::get_tzdb();
    } catch (const std::runtime_error& ex) {
        LOG_FATAL("Error loading bundled Time Zone Database: {}", ex.what());
        std::terminate();
    }
#endif
}

date::zoned_seconds core::getISTDateTime(const std::string& gmtDate) {
    std::stringstream ss{gmtDate};
    date::sys_seconds gmtTimePoint;
    ss >> date::parse("%F %T", gmtTimePoint);
    if (ss.fail()) {
        LOG_FATAL("Failed to parse string date: {}", gmtDate);
        PRINT_FATAL("Failed to parse string date: {}", gmtDate);
        throw std::invalid_argument("Failed to parse string date: " + gmtDate);
    }
    return date::make_zoned("Asia/Kolkata", gmtTimePoint);
}

date::zoned_seconds core::getISTDatetimeFromUnix(const int64_t epoch) {
    const auto tp = date::sys_seconds{std::chrono::seconds{epoch}};
    return date::make_zoned("Asia/Kolkata", tp);
}
