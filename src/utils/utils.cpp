#include "core/utils/utils.hpp"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <stdexcept>

#include "core/logger/manager/manager.hpp"

size_t core::CaseInsensitiveHash::operator()(const std::string_view key) const {
    return caseInsensitiveHash(key);
}

bool core::CaseInsensitiveEqual::operator()(const std::string_view a, const std::string_view b) const {
    return isEqualCaseInsensitive(a, b);
}

char core::toLower(const char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + 32;
    }
    return c;
}

size_t core::caseInsensitiveHash(const std::string_view key) {
    std::size_t h = 2166136261u;
    for (char c : key) {
        h ^= static_cast<unsigned char>(core::toLower(c));
        h *= 16777619u;
    }
    return h;
}

bool core::isEqualCaseInsensitive(const std::string_view a, const std::string_view b) {
    return std::equal(a.begin(), a.end(), b.begin(), b.end(),
        [](const char ca, const char cb) { return toLower(ca) == toLower(cb); });
}

void core::printError(const std::string& task, const DWORD errorCode) {
    LOG_ERROR("{} failed. Error Code: {}", task, errorCode);
}

std::wstring core::toWideString(const std::string& input) {
    if (input.empty()) {
        return L"";
    }

    if (input.size() > static_cast<size_t>((std::numeric_limits<int>::max)())) {
        throw std::runtime_error("Input string too long for conversion.");
    }

    const int inputSize = static_cast<int>(input.size());

    const int sizeNeeded = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, input.data(), inputSize, nullptr, 0);
    if (sizeNeeded <= 0) {
        if (const DWORD error = GetLastError(); error == ERROR_NO_UNICODE_TRANSLATION) {
            throw std::runtime_error("Invalid UTF-8 sequence detected");
        }
        throw std::runtime_error("Failed to calculate wide string size");
    }

    std::wstring result(sizeNeeded, 0);
    MultiByteToWideChar(CP_UTF8, 0, input.data(), inputSize, &result[0], sizeNeeded);
    return result;
}

std::string core::urlEncode(const std::string& input) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex << std::uppercase;

    for (auto it = input.begin(); it != input.end(); ++it) {
        // Keep alphanumeric and other safe characters as defined by RFC 3986
        if (const auto c = static_cast<unsigned char>(*it);
            std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << static_cast<char>(c);
        } else {
            // Any other character becomes %XX
            // This correctly handles UTF-8 multi-byte sequences by encoding each byte
            escaped << '%' << std::setw(2) << static_cast<int>(c);
        }
    }

    return escaped.str();
}

std::wstring core::generateURLEndpoint(
    const std::wstring& path, const std::multimap<std::string, std::string>& params) {
    if (path.empty()) {
        return L"/";
    }
    std::ostringstream res;
    char delimiter = '?';
    for (const auto& [key, value] : params) {
        res << delimiter << urlEncode(key) << '=' << urlEncode(value);
        delimiter = '&';
    }
    const auto queries = toWideString(res.str());

    return path + queries;
}
