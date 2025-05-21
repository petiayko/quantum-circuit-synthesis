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

    size_t size() const noexcept;

    size_t dim() const noexcept;

    size_t weight() const noexcept;

    bool is_balanced() const noexcept;

    std::vector<bool> mobius_transformation() const noexcept;

    std::vector<int> RW_spectrum() const noexcept;

    int adjacent_zeros() const noexcept;

    int complexity() const noexcept;

    BinaryMapping extend() const;

    binary_vector vector() const noexcept;

    std::string to_table(char = '\t') const noexcept;

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

    cf_set coordinate_functions() const noexcept;

    size_t inputs_number() const noexcept;

    size_t outputs_number() const noexcept;

    bool is_substitution() const noexcept;

    BinaryMapping extend() const;

    std::string to_table(char = '\t') const noexcept;

    friend std::ostream &operator<<(std::ostream &, const BinaryMapping &) noexcept;

private:
    cf_set cf_;

    table to_table_() const noexcept;

    void by_string_(const std::string &);
};

class Substitution {
public:
    explicit Substitution(const cf_set &);

    explicit Substitution(const table &);

    explicit Substitution(const std::string &);

    explicit Substitution(std::istream &);

    Substitution(const Substitution &);

    Substitution(const BinaryMapping &);

    Substitution &operator=(const Substitution &);

    Substitution &operator=(const BinaryMapping &);

    bool operator==(const Substitution &) const;

    bool operator!=(const Substitution &) const;

    size_t power() const noexcept;

    bool is_identical() const noexcept;

    std::vector<size_t> vector() const noexcept;

    std::vector<std::pair<size_t, size_t>> transpositions() const noexcept;

    std::vector<std::vector<size_t>> cycles() const noexcept;

    bool is_odd() const noexcept;

    friend std::ostream &operator<<(std::ostream &, const Substitution &) noexcept;

private:
    std::vector<size_t> sub_;

    void by_string_(const std::string &);
};

#endif //QUANTUM_CIRCUIT_SYNTHESIS_PRIMITIVES_HPP
