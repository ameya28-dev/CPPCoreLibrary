#ifndef CORELIBRARY_INCLUDE_CORE_DATE_DATE_HPP
#define CORELIBRARY_INCLUDE_CORE_DATE_DATE_HPP

#include <date/date.h>
#include <date/tz.h>
#include <fmt/base.h>

template <>
struct fmt::formatter<date::zoned_seconds> {
    static constexpr auto parse(const fmt::format_parse_context& context) {
        return context.begin();
    }

    static auto format(const date::zoned_seconds& zt, const fmt::format_context& context) {
        return fmt::format_to(context.out(), "{}", date::format("%d-%M-%Y %H:%M:%S", zt));
    }
};

namespace core {
    namespace constants {
        struct Patterns {
            static constexpr auto DateTime = "%d-%m-%Y %H:%M:%S";
            static constexpr auto Date     = "%d-%m-%Y";
            static constexpr auto Time     = "%H:%M:%S";
            static constexpr auto Time12Hr = "%I:%M:%S %p";
        };
    } // namespace constants

    void setTimeZoneDB();

    date::zoned_seconds getISTDateTime(const std::string& gmtDate);

    date::zoned_seconds getISTDatetimeFromUnix(int64_t epoch);

} // namespace core


#endif // CORELIBRARY_INCLUDE_CORE_DATE_DATE_HPP
