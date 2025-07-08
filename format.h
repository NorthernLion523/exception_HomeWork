
#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <type_traits>
#include <ostream>

// Трейт: проверяем, что T поддерживает operator<<
template<typename T, typename = void>
struct is_streamable : std::false_type {};

template<typename T>
struct is_streamable<
    T,
    std::void_t<decltype(std::declval<std::ostream&>() << std::declval<T>())>
> : std::true_type {};

class FormatException : public std::runtime_error {
public:
    explicit FormatException(const std::string& msg)
        : std::runtime_error("Format error: " + msg) {}
};

template<typename T>
std::string toString(T const& v) {
    static_assert(is_streamable<T>::value,
                  "Type must be streamable (operator<<)");
    std::ostringstream oss;
    oss << v;
    return oss.str();
}

template<typename... Args>
std::string format(const std::string& fmt, Args const&... args) {
    std::vector<std::string> a{ toString(args)... };
    std::string out;
    for (size_t i = 0; i < fmt.size(); ++i) {
        if (fmt[i] == '{') {
            auto close = fmt.find('}', i + 1);
            if (close == std::string::npos)
                throw FormatException("no closing brace");
            auto idxStr = fmt.substr(i + 1, close - i - 1);
            if (idxStr.empty())
                throw FormatException("empty braces");
            for (char c : idxStr)
                if (!isdigit(c))
                    throw FormatException("non-digit in braces");
            size_t idx = std::stoul(idxStr);
            if (idx >= a.size())
                throw FormatException("index out of range");
            out += a[idx];
            i = close;
        }
        else if (fmt[i] == '}') {
            throw FormatException("unmatched closing brace");
        }
        else {
            out += fmt[i];
        }
    }
    return out;
}