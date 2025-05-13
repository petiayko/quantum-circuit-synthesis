#ifndef QUANTUM_CIRCUIT_SYNTHESIS_STRINGS_HPP
#define QUANTUM_CIRCUIT_SYNTHESIS_STRINGS_HPP

#include <algorithm>
#include <string>
#include <vector>

inline void trim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](char ch) {
        return !std::isspace(ch);
    }));
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

template<typename T = size_t>
inline T strict_stoi(const std::string &s, int base = 10) {
    if (s.empty()) {
        throw std::runtime_error{"Empty string"};
    }

    size_t pos;
    T value;

    try {
        value = std::stoi(s, &pos, base);
    } catch (const std::invalid_argument &) {
        throw std::runtime_error{std::string("Invalid number: ") + s};
    } catch (const std::out_of_range &) {
        throw std::runtime_error{std::string("Number out of range: ") + s};
    }
    while (pos < s.size() && std::isspace(s[pos])) {
        pos++;
    }
    if (pos != s.size()) {
        throw std::runtime_error{std::string("Extra characters after number: ") + s};
    }
    return value;
}

template<typename T = size_t>
inline bool try_string_to_decimal(const std::string &s, T &num) {
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
            throw std::runtime_error{std::string("Invalid num: ") + num_s};
        }
        result.push_back(num);
    }
    return result;
}

#endif //QUANTUM_CIRCUIT_SYNTHESIS_STRINGS_HPP
