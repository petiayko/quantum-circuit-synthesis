#ifndef QUANTUM_CIRCUIT_SYNTHESIS_MATH_HPP
#define QUANTUM_CIRCUIT_SYNTHESIS_MATH_HPP

#include <algorithm>
#include <cmath>
#include <string>

template<class T = size_t>
bool is_power_of_2(T v) {
    return v && !(v & (v - 1));
}

template<class T = size_t>
std::string decimal_to_binary(T v, size_t l = 0) {
    std::string result;
    if (!v) {
        result = "0";
    }
    while (v) {
        result += (v % 2 ? '1' : '0');
        v /= 2;
    }
    if (l && l > result.size()) {
        result += std::string(l - result.size(), '0');
    }
    std::reverse(result.begin(), result.end());
    return result;
}

template<class T = size_t>
T binary_to_decimal(const std::string &s) {
    if (s.empty()) {
        return 0;
    }
    for (auto bit: s) {
        if (bit == '0' || bit == '1') {
            continue;
        }
        throw std::runtime_error{"Unexpected symbol in binary notation"};
    }
    return std::stoi(s, nullptr, 2);
}


#endif //QUANTUM_CIRCUIT_SYNTHESIS_MATH_HPP
