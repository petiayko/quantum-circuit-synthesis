#ifndef QUANTUM_CIRCUIT_SYNTHESIS_MATH_HPP
#define QUANTUM_CIRCUIT_SYNTHESIS_MATH_HPP

#include <algorithm>
#include <bitset>
#include <cmath>
#include <string>

#include "exseptions.hpp"


template<class T = size_t>
inline constexpr bool is_power_of_2(T v) noexcept {
    if constexpr (std::is_unsigned_v<T>) {
        return v != 0 && std::has_single_bit(v);
    }
    return v > 0 && !(v & (v - 1));
}

template<class T = size_t>
inline std::string decimal_to_binary_s(T v, size_t l = 0) noexcept {
    constexpr size_t bits_count = sizeof(T) * 8;
    std::bitset<bits_count> bits(v);

    std::string result;
    bool found_one = false;

    for (int i = bits_count - 1; i >= 0; i--) {
        if (bits[i] || found_one) {
            result += bits[i] ? '1' : '0';
            found_one = true;
        }
    }

    if (result.empty()) {
        result = "0";
    }
    if (l > result.size()) {
        result = std::string(l - result.size(), '0') + result;
    }

    return result;
}

template<class T = size_t>
inline std::vector<bool> decimal_to_binary_v(T v, size_t l = 0) noexcept {
    std::vector<bool> result;
//    result.reserve(std::max(sizeof(T) * 8, l));
    if (!v) {
        result.push_back(false);
    } else {
        while (v) {
            result.insert(result.begin(), v & 1);
            v >>= 1;
        }
    }
    if (l > result.size()) {
        result.insert(result.begin(), l - result.size(), false);
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
inline size_t binary_dot(T a, T b) noexcept {
    if constexpr (std::is_unsigned_v<T>) {
        return std::popcount(static_cast<std::make_unsigned_t<T>>(a & b));
    }
    auto mask = a & b;
    size_t count = 0;
    while (mask) {
        mask &= (mask - 1);
        ++count;
    }
    return count;
}

template<typename T = size_t>
inline std::vector<size_t> bits_mask(T a, int base) noexcept {
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
