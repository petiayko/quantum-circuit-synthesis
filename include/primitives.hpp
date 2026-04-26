#ifndef QUANTUM_CIRCUIT_SYNTHESIS_PRIMITIVES_HPP
#define QUANTUM_CIRCUIT_SYNTHESIS_PRIMITIVES_HPP

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

#include "exseptions.hpp"
#include "math.hpp"
#include "strings.hpp"


using binary_vector = std::vector<bool>;
using table = std::vector<binary_vector>;

class BinaryMapping;

class BooleanFunction {
public:
    explicit BooleanFunction() = default;

    explicit BooleanFunction(size_t, size_t);

    explicit BooleanFunction(bool, size_t);

    explicit BooleanFunction(const binary_vector &);

    explicit BooleanFunction(const std::vector<int> &);

    explicit BooleanFunction(const std::string &);

    BooleanFunction(const BooleanFunction &);

    BooleanFunction &operator=(const BooleanFunction &);

    bool operator==(const BooleanFunction &) const;

    bool operator!=(const BooleanFunction &) const;

    BooleanFunction &operator+=(const BooleanFunction &);

    BooleanFunction &operator*=(const BooleanFunction &);

    BooleanFunction &operator|=(const BooleanFunction &);

    BooleanFunction &operator~() noexcept;

    explicit operator bool() const;

    [[nodiscard]] size_t size() const noexcept;

    [[nodiscard]] size_t dim() const noexcept;

    [[nodiscard]] size_t weight() const noexcept;

    [[nodiscard]] bool is_balanced() const noexcept;

    [[nodiscard]] bool is_constant() const noexcept;

    [[nodiscard]] size_t variable() const;

    [[nodiscard]] std::vector<bool> mobius_transformation() const noexcept;

    [[nodiscard]] std::vector<int> RW_spectrum() const noexcept;

    [[nodiscard]] int adjacent_zeros() const noexcept;

    [[nodiscard]] int complexity() const noexcept;

    [[nodiscard]] BinaryMapping extend() const;

    [[nodiscard]] binary_vector vector() const noexcept;

    [[nodiscard]] std::string to_table(char = '\t') const noexcept;

    friend std::ostream &operator<<(std::ostream &, const BooleanFunction &) noexcept;

private:
    binary_vector vec_;
};

BooleanFunction operator+(const BooleanFunction &, const BooleanFunction &);

BooleanFunction operator*(const BooleanFunction &, const BooleanFunction &);

BooleanFunction operator|(const BooleanFunction &, const BooleanFunction &);

using cf_set = std::vector<BooleanFunction>;

class Substitution;

class BinaryMapping {
public:
    explicit BinaryMapping(const cf_set &);

    explicit BinaryMapping(const table &);

    explicit BinaryMapping(const std::string &);

    explicit BinaryMapping(std::istream &);

    BinaryMapping(const BinaryMapping &);

    BinaryMapping(const Substitution &);

    BinaryMapping &operator=(const BinaryMapping &);

    BinaryMapping &operator=(const Substitution &);

    bool operator==(const BinaryMapping &) const;

    bool operator!=(const BinaryMapping &) const;

    bool operator==(const Substitution &) const;

    bool operator!=(const Substitution &) const;

    [[nodiscard]] cf_set coordinate_functions() const noexcept;

    [[nodiscard]] size_t inputs_number() const noexcept;

    [[nodiscard]] size_t outputs_number() const noexcept;

    [[nodiscard]] bool is_substitution() const noexcept;

    [[nodiscard]] BinaryMapping extend() const;

    [[nodiscard]] std::string to_table(char = '\t') const noexcept;

    friend std::ostream &operator<<(std::ostream &, const BinaryMapping &) noexcept;

private:
    cf_set cf_;

    [[nodiscard]] table to_table_() const noexcept;

    void by_string_(const std::string &);
};

using cycle_type = std::vector<size_t>;
using transposition_type = std::pair<size_t, size_t>;

size_t cayley_distance(const Substitution &, const Substitution &);

class Substitution {
public:
    explicit Substitution(const std::vector<size_t> &);

    explicit Substitution(const std::vector<cycle_type> &);

    explicit Substitution(const std::vector<transposition_type> &);

    explicit Substitution(const cf_set &);

    explicit Substitution(const table &);

    explicit Substitution(const std::string &);

    explicit Substitution(size_t);

    explicit Substitution(std::istream &);

    Substitution(const BinaryMapping &);

    Substitution(const Substitution &);

    Substitution &operator=(const Substitution &);

    Substitution &operator=(const BinaryMapping &);

    bool operator==(const Substitution &) const;

    bool operator!=(const Substitution &) const;

    bool operator==(const BinaryMapping &) const;

    bool operator!=(const BinaryMapping &) const;

    Substitution &operator*=(const Substitution &);

    [[nodiscard]] size_t power() const noexcept;

    [[nodiscard]] bool is_identical() const noexcept;

    [[nodiscard]] std::vector<size_t> vector() const noexcept;

    [[nodiscard]] std::vector<transposition_type> transpositions() const noexcept;

    [[nodiscard]] std::vector<cycle_type> cycles() const noexcept;

    [[nodiscard]] Substitution invert() const noexcept;

    [[nodiscard]] bool is_odd() const noexcept;

    friend std::ostream &operator<<(std::ostream &, const Substitution &) noexcept;

    friend Substitution substitution_by_cycle(const cycle_type &);

    friend Substitution substitution_power_of_2_by_cycle(const cycle_type &);

private:
    std::vector<size_t> sub_;

    void by_string_(const std::string &);
};

Substitution operator*(const Substitution &, const Substitution &);

Substitution substitution_by_cycle(const cycle_type &);

Substitution substitution_power_of_2_by_cycle(const cycle_type &);

#endif //QUANTUM_CIRCUIT_SYNTHESIS_PRIMITIVES_HPP
