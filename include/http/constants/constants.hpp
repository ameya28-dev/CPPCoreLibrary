#ifndef CORELIBRARY_INCLUDE_HTTP_CONSTANTS_CONSTANTS_HPP
#define CORELIBRARY_INCLUDE_HTTP_CONSTANTS_CONSTANTS_HPP

namespace core::constants {
    struct Defaults {
        static constexpr auto Verbose             = false;
        static constexpr auto RetryCount          = 3;
        static constexpr auto JSONApplicationType = "application/json";
    };

    struct Keys {
        static constexpr auto ContentType   = "content-type";
        static constexpr auto Authorization = "authorization";
    };

    struct Jitter {
        static constexpr auto Min = 100;
        static constexpr auto Max = 900;
    };
} // namespace core::constants

#endif // CORELIBRARY_INCLUDE_HTTP_CONSTANTS_CONSTANTS_HPP
