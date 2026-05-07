#ifndef CORELIBRARY_INCLUDE_CORE_INITIALIZER_HPP
#define CORELIBRARY_INCLUDE_CORE_INITIALIZER_HPP

#include "date/date.hpp"
#include "logger/manager/manager.hpp"

namespace core {
    inline void initialize() {
        LOG_INIT();
        setTimeZoneDB();
    }
} // namespace core

#endif // CORELIBRARY_INCLUDE_CORE_INITIALIZER_HPP
