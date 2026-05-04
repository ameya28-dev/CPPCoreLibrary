#ifndef CORELIBRARY_INCLUDE_CORE_LOGGER_CONSTANTS_CONSTANTS_HPP
#define CORELIBRARY_INCLUDE_CORE_LOGGER_CONSTANTS_CONSTANTS_HPP

namespace core::constants {
    enum class ANSIColor {
        black          = 30,
        red            = 31,
        green          = 32,
        yellow         = 33,
        blue           = 34,
        magenta        = 35,
        cyan           = 36,
        white          = 37,
        bright_black   = 90,
        bright_red     = 91,
        bright_green   = 92,
        bright_yellow  = 93,
        bright_blue    = 94,
        bright_magenta = 95,
        bright_cyan    = 96,
        bright_white   = 97,
    };

    namespace Log {
        static constexpr int MaxFileSize      = 5 * 1024 * 1024;
        static constexpr int MaxRotatingFiles = 3;

        struct Type {
            static constexpr auto Core  = "CORE";
            static constexpr auto Print = "PRINT";
        };

        struct Color {
            // Formatting codes
            static constexpr auto Reset     = "\033[m";
            static constexpr auto Bold      = "\033[1m";
            static constexpr auto Dark      = "\033[2m";
            static constexpr auto Underline = "\033[4m";
            static constexpr auto Blink     = "\033[5m";
            static constexpr auto Reverse   = "\033[7m";
            static constexpr auto Concealed = "\033[8m";
            static constexpr auto ClearLine = "\033[K";

            // Foreground colors
            static constexpr auto Black   = "\033[30m";
            static constexpr auto Red     = "\033[31m";
            static constexpr auto Green   = "\033[32m";
            static constexpr auto Yellow  = "\033[33m";
            static constexpr auto Blue    = "\033[34m";
            static constexpr auto Magenta = "\033[35m";
            static constexpr auto Cyan    = "\033[36m";
            static constexpr auto White   = "\033[37m";

            /// Background colors
            static constexpr auto OnBlack   = "\033[40m";
            static constexpr auto OnRed     = "\033[41m";
            static constexpr auto OnGreen   = "\033[42m";
            static constexpr auto OnYellow  = "\033[43m";
            static constexpr auto OnBlue    = "\033[44m";
            static constexpr auto OnMagenta = "\033[45m";
            static constexpr auto OnCyan    = "\033[46m";
            static constexpr auto OnWhite   = "\033[47m";

            /// Bold colors
            static constexpr auto YellowBold = "\033[33m\033[1m";
            static constexpr auto RedBold    = "\033[31m\033[1m";
            static constexpr auto BoldOnRed  = "\033[1m\033[41m";
        };

        namespace Pattern {
            struct Print {
                static constexpr auto Console = "%^[%n] [%l] %v%$";
            };

            struct Core {
                static constexpr auto Console = "%^[%T] [%n] [%l] %v%$";
                static constexpr auto File    = "[%Y-%m-%d %H:%M:%S.%e] [%n] [%l] %v";
            };
        } // namespace Pattern
    } // namespace Log
} // namespace core::constants


#endif // CORELIBRARY_INCLUDE_CORE_LOGGER_CONSTANTS_CONSTANTS_HPP
