#ifndef QUANTUM_CIRCUIT_SYNTHESIS_PRIMITIVES_HPP
#define QUANTUM_CIRCUIT_SYNTHESIS_PRIMITIVES_HPP

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>
#include <numeric>

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

class BooleanFunction {
public:
    explicit BooleanFunction(const std::vector<bool> &);

    explicit BooleanFunction(const std::vector<int> &);

    explicit BooleanFunction(const std::string &);

    BooleanFunction(const BooleanFunction &);

    BooleanFunction &operator=(const BooleanFunction &);

    bool operator==(const BooleanFunction &) const;

    bool operator!=(const BooleanFunction &) const;

    size_t size() const noexcept;

    size_t dim() const noexcept;

    size_t weight() const noexcept;

    bool is_balanced() const noexcept;

    std::vector<bool> get_vector() const noexcept;

    std::string to_table(char = '\t') const noexcept;

    friend std::ostream &operator<<(std::ostream &, const BooleanFunction &) noexcept;

private:
    std::vector<bool> v_;
};

//using substitution = std::vector<size_t>;
using table = std::vector<std::vector<bool>>;

//bool is_substitution(const substitution &);

//substitution table_to_substitution(const binary_substitution &);

//binary_substitution substitution_to_table(const substitution &);

class Mapping {
public:
    explicit Mapping(const table &);

    explicit Mapping(const std::vector<BooleanFunction> &);

    explicit Mapping(const std::string &);

    Mapping(const Mapping &sub);

    Mapping &operator=(const Mapping &);

    bool operator==(const Mapping &) const;

    bool operator!=(const Mapping &) const;

    std::vector<BooleanFunction> get_coordinate_functions() const noexcept;

    bool is_substitution() const noexcept;

    size_t get_inputs_number() const noexcept;

    size_t get_outputs_number() const noexcept;

private:
    table table_;
};

class Substitution : public Mapping {

};

#endif //QUANTUM_CIRCUIT_SYNTHESIS_PRIMITIVES_HPP
