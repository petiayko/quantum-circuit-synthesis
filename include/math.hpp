#ifndef QUANTUM_CIRCUIT_SYNTHESIS_MATH_HPP
#define QUANTUM_CIRCUIT_SYNTHESIS_MATH_HPP

#include <algorithm>
#include <cmath>
#include <string>

#include "exseptions.hpp"


template<class T = size_t>
inline bool is_power_of_2(T v) {
    return v && !(v & (v - 1));
}

template<class T = size_t>
inline std::string decimal_to_binary_s(T v, size_t l = 0) {
    std::string result;
    if (!v) {
        result = "0";
    } else {
        while (v) {
            result += (v % 2 ? '1' : '0');
            v /= 2;
        }
    }
    if (l > result.size()) {
        result += std::string(l - result.size(), '0');
    }
    std::reverse(result.begin(), result.end());
    return result;
}

template<class T = size_t>
inline std::vector<bool> decimal_to_binary_v(T v, size_t l = 0) {
    std::vector<bool> result;
    if (!v) {
        result = {false};
    } else {
        while (v) {
            result.insert(result.begin(), v % 2);
            v /= 2;
        }
    }
    if (l > result.size()) {
        size_t r_size = result.size();
        for (size_t _ = 0; _ < l - r_size; _++) {
            result.insert(result.begin(), false);
        }
    }
    return result;
}

template<class T = size_t>
inline T binary_to_decimal(const std::string &s) {
    if (s.empty()) {
        return 0;
    }
    for (auto bit: s) {
        if (bit == '0' || bit == '1') {
            continue;
        }
        throw MathException(("Unexpected symbol in binary notation: ") + bit);
    }
    return std::stoi(s, nullptr, 2);
}


template<typename T>
inline size_t binary_dot(T a, T b) {
    int mask = a & b;
    int count = 0;
    while (mask) {
        count += mask & 1;
        mask >>= 1;
    }
    return count;
}

template<typename T = size_t>
inline std::vector<size_t> bits_mask(T a, int base) {
    std::vector<size_t> bits;
    auto a_binary = decimal_to_binary_s<T>(a, base);
    for (size_t i = 0; i < a_binary.size(); i++) {
        if (a_binary[i] == '1') {
            bits.push_back(i);
        }
    }
    return bits;
}

#endif //QUANTUM_CIRCUIT_SYNTHESIS_MATH_HPP
