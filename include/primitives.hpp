#ifndef QUANTUM_CIRCUIT_SYNTHESIS_PRIMITIVES_HPP
#define QUANTUM_CIRCUIT_SYNTHESIS_PRIMITIVES_HPP

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include <sstream>
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

//using substitution = std::vector<size_t>;
using binary_vector = std::vector<bool>;
using table = std::vector<binary_vector>;

class BooleanFunction {
public:
    explicit BooleanFunction(const binary_vector &);

    explicit BooleanFunction(const std::vector<int> &);

    explicit BooleanFunction(const std::string &);

    BooleanFunction(const BooleanFunction &);

    BooleanFunction &operator=(const BooleanFunction &);

    bool operator==(const BooleanFunction &) const;

    bool operator!=(const BooleanFunction &) const;

    BooleanFunction &operator+=(const BooleanFunction &);

    size_t size() const noexcept;

    size_t dim() const noexcept;

    size_t weight() const noexcept;

    bool is_balanced() const noexcept;

    binary_vector get_vector() const noexcept;

    std::string to_table(char = '\t') const noexcept;

    friend std::ostream &operator<<(std::ostream &, const BooleanFunction &) noexcept;

private:
    binary_vector vec_;
};

BooleanFunction operator+(const BooleanFunction &, const BooleanFunction &);

using cf_set = std::vector<BooleanFunction>;

//bool is_substitution(const substitution &);

//substitution table_to_substitution(const binary_substitution &);

//binary_substitution substitution_to_table(const substitution &);

class Substitution;

class BinaryMapping {
public:
    explicit BinaryMapping(const cf_set &);

    explicit BinaryMapping(const table &);

    explicit BinaryMapping(const std::string &);

    explicit BinaryMapping(const Substitution &);

    BinaryMapping(const BinaryMapping &sub);

    BinaryMapping &operator=(const BinaryMapping &);

    BinaryMapping &operator=(const Substitution &);

    bool operator==(const BinaryMapping &) const;

    bool operator!=(const BinaryMapping &) const;

    cf_set get_coordinate_functions() const noexcept;

    bool is_substitution() const noexcept;

    size_t get_inputs_number() const noexcept;

    size_t get_outputs_number() const noexcept;

    std::string to_table(char = '\t') const noexcept;

    friend std::ostream &operator<<(std::ostream &, const BinaryMapping &) noexcept;

protected:
    table to_table_() const noexcept;

private:
    cf_set cf_;
};

class Substitution : public BinaryMapping {
public:
    explicit Substitution(const cf_set &);

    explicit Substitution(const table &);

    explicit Substitution(const std::string &);

    Substitution(const Substitution &);

    Substitution &operator=(const Substitution &);

    Substitution &operator=(const BinaryMapping &);

    size_t power() const noexcept;

    std::vector<size_t> get_vector() const noexcept;

    bool is_odd() const noexcept;

    friend std::ostream &operator<<(std::ostream &, const Substitution &) noexcept;

private:
    std::vector<size_t> sub_;
};

#endif //QUANTUM_CIRCUIT_SYNTHESIS_PRIMITIVES_HPP
