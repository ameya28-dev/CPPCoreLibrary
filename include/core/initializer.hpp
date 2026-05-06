#ifndef WINHTTPPROJECT_CPPCORELIBRARY_INCLUDE_CORE_INITIALIZER_HPP
#define WINHTTPPROJECT_CPPCORELIBRARY_INCLUDE_CORE_INITIALIZER_HPP

#include "date/date.hpp"
#include "logger/manager/manager.hpp"

namespace core {
    inline void initialize() {
        LOG_INIT();
        setTimeZoneDB();
    }
} // namespace core

#endif // WINHTTPPROJECT_CPPCORELIBRARY_INCLUDE_CORE_INITIALIZER_HPP
