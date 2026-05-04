#ifndef CORELIBRARY_INCLUDE_WIN_HTTP_CONSTANTS_CONSTANTS_HPP
#define CORELIBRARY_INCLUDE_WIN_HTTP_CONSTANTS_CONSTANTS_HPP

#include <chrono>

namespace core {
    using namespace std::chrono_literals;

    namespace constants {
        static constexpr int EightKBSize = 1024 * 8;

        struct Timeouts {
            static constexpr std::chrono::milliseconds Connect = 5s;
            static constexpr std::chrono::milliseconds Send    = 5s;
            static constexpr std::chrono::milliseconds Receive = 10s;
        };

    } // namespace constants

} // namespace core

#endif // CORELIBRARY_INCLUDE_WIN_HTTP_CONSTANTS_CONSTANTS_HPP
