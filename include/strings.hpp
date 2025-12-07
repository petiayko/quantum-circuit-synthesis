#ifndef QUANTUM_CIRCUIT_SYNTHESIS_STRINGS_HPP
#define QUANTUM_CIRCUIT_SYNTHESIS_STRINGS_HPP

#include <algorithm>
#include <charconv>
#include <string>
#include <system_error>
#include <vector>

#include "exseptions.hpp"


inline void trim(std::string &s) noexcept {
    size_t start = 0;
    while (start < s.size() && std::isspace(s[start])) {
        ++start;
    }
    size_t end = s.size();
    while (end > start && std::isspace(s[end - 1])) {
        --end;
    }
    if (start > 0 || end < s.size()) {
        s = s.substr(start, end - start);
    }
}

template<typename T = size_t>
inline T strict_stoi(const std::string &s, int base = 10) {
    if (s.empty()) {
        throw StringException("Empty string");
    }

    std::string_view sv(s);

    size_t start = 0;
    while (start < sv.size() && std::isspace(sv[start])) {
        ++start;
    }
    if (start == sv.size()) {
        throw StringException("String contains only spaces");
    }

    T value;
    auto begin = sv.data() + start;
    auto end = sv.data() + sv.size();
    auto [ptr, ec] = std::from_chars(begin, end, value, base);

    if (ec == std::errc::invalid_argument) {
        throw StringException("Invalid number: " + std::string(sv.substr(start)));
    }

    if (ec == std::errc::result_out_of_range) {
        throw StringException("Number out of range: " + std::string(sv.substr(start)));
    }

    while (ptr < end && std::isspace(*ptr)) {
        ++ptr;
    }
    if (ptr != end) {
        throw StringException("Extra characters after number: " + std::string(sv));
    }
    return value;
}

template<typename T = size_t>
inline bool try_string_to_decimal(const std::string &s, T &num) noexcept {
    if (s.empty()) {
        return false;
    }
    if (!(s.rfind("0x") && s.rfind("0X"))) {
        try {
            std::stringstream hex_value_ss;
            hex_value_ss << std::hex << s;
            hex_value_ss >> num;
            return true;
        } catch (...) {
            return false;
        }
    }
    try {
        num = strict_stoi(s);
        return true;
    } catch (...) {
        try {
            num = strict_stoi(s, 16);
            return true;
        } catch (...) {
            return false;
        }
    }
}

template<typename T = size_t>
inline std::vector<T> string_to_num_vector(const std::string &s, char sep = ' ') {
    std::vector<T> result;

    std::stringstream ss;
    ss << s;
    std::string num_s;
    T num;
    while (getline(ss, num_s, sep)) {
        trim(num_s);
        if (num_s.empty()) {
            continue;
        }
        if (!try_string_to_decimal<T>(num_s, num)) {
            throw StringException("Invalid num: " + num_s);
        }
        result.push_back(num);
    }
    return result;
}

inline void to_lower(std::string &s) {
    for (auto &ch: s) {
        if (ch >= 'A' && ch <= 'Z') {
            ch += ('a' - 'A');
        }
    }
}

#endif //QUANTUM_CIRCUIT_SYNTHESIS_STRINGS_HPP
