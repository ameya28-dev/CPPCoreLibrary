#ifndef CORELIBRARY_INCLUDE_CORE_WIN_HTTP_UTILS_UTILS_HPP
#define CORELIBRARY_INCLUDE_CORE_WIN_HTTP_UTILS_UTILS_HPP

#include <map>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>

#if WIN32
#include <windows.h>
#endif

#ifdef __has_include
#if __has_include(<nlohmann/json.hpp>)
#include <type_traits>

#include <nlohmann/json.hpp>

template <class... Ts>
struct Overloaded : Ts... {
    using Ts::operator()...;
};

template <class... Ts>
Overloaded(Ts...) -> Overloaded<Ts...>;

namespace core {
    template <typename T, typename = void>
    struct IsJsonDeserializable : std::false_type {};

    template <typename T>
    struct IsJsonDeserializable<T, std::void_t<decltype(std::declval<nlohmann::json>().get<T>())>> : std::true_type {};

    template <typename T, typename = void>
    struct IsJsonSerializable : std::false_type {};

    template <typename T>
    struct IsJsonSerializable<T, std::void_t<decltype(nlohmann::json(std::declval<T>()))>> : std::true_type {};

    template <typename T>
    T tryParse(const std::string& str) {
        if constexpr (std::is_same_v<T, std::string>) {
            return str;
        }

        if (str.empty()) {
            return T{};
        }

        nlohmann::json j = nlohmann::json::parse(str);
        return j.get<T>();
    }
} // namespace core

#endif
#endif

namespace core {
    struct CaseInsensitiveHash {
        size_t operator()(std::string_view) const;
    };

    struct CaseInsensitiveEqual {
        bool operator()(std::string_view, std::string_view) const;
    };

    using Headers = std::unordered_multimap<std::string, std::string, CaseInsensitiveHash, CaseInsensitiveEqual>;
    using Params  = std::multimap<std::string, std::string>;

    // Fast ASCII toLower
    char toLower(char c);

    size_t caseInsensitiveHash(std::string_view);

    bool isEqualCaseInsensitive(std::string_view, std::string_view);

    std::string urlEncode(const std::string&);

#if WIN32
    void printError(const std::string&, DWORD);

    std::wstring toWideString(const std::string&);

    std::wstring generateURLEndpoint(const std::wstring&, const std::multimap<std::string, std::string>&);
#endif

    template <typename... Ts>
    auto makeVisitor(Ts&&... ts) {
        return Overloaded<std::decay_t<Ts>...>{std::forward<Ts>(ts)...};
    }

    template <typename Variant, typename... Handlers>
    auto match(Variant&& v, Handlers&&... handlers) {
        return std::visit(makeVisitor(std::forward<Handlers>(handlers)...), std::forward<Variant>(v));
    }
} // namespace core

#endif // CORELIBRARY_INCLUDE_CORE_WIN_HTTP_UTILS_UTILS_HPP
