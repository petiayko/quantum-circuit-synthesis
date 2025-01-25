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

class BooleanFunction {
public:
    explicit BooleanFunction(const std::vector<bool> &v) {
        if (!is_power_of_2(v.size())) {
            throw std::runtime_error{"Invalid length"};
        }
        v_ = v;
    }

    explicit BooleanFunction(const std::vector<int> &v) {
        if (!is_power_of_2(v.size())) {
            throw std::runtime_error{"Invalid length"};
        }
        for (auto i: v) {
            if (i == 1) {
                v_.push_back(true);
            } else if (!i) {
                v_.push_back(false);
            } else {
                throw std::runtime_error{"Unexpected value"};
            }
        }
    }

    explicit BooleanFunction(const std::string &s) {
        if (!is_power_of_2(s.size())) {
            throw std::runtime_error{"Invalid length"};
        }
        for (auto i: s) {
            if (i == '1') {
                v_.push_back(true);
            } else if (i == '0') {
                v_.push_back(false);
            } else {
                throw std::runtime_error{"Unexpected value"};
            }
        }
    }

    BooleanFunction(const BooleanFunction &bf) {
        v_ = bf.v_;
    }

    ~BooleanFunction() {
        v_.clear();
    }

    BooleanFunction &operator=(const BooleanFunction &bf) {
        if (this->operator!=(bf)) {
            v_.clear();
            v_ = bf.v_;
        }
        return *this;
    }

    bool operator==(const BooleanFunction &bf) const {
        if (this->size() != bf.size()) {
            return false;
        }
        for (size_t i = 0; i < this->size(); i++) {
            if (v_[i] != bf.v_[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const BooleanFunction &bf) const {
        return !this->operator==(bf);
    }

    size_t size() const noexcept {
        return v_.size();
    }

    size_t dim() const noexcept {
        return lround(log2(this->size()));
    }

    size_t weight() const noexcept {
        return std::accumulate(v_.begin(), v_.end(), 0);
    }

    bool is_balanced() const noexcept {
        return this->weight() * 2 == this->size();
    }

    std::string to_table(char sep = '\t') const noexcept {
        std::string out;
        std::string set;
        for (size_t i = 0; i < this->size(); i++) {
            out += decimal_to_binary(i, this->dim()) + sep + (v_[i] ? '1' : '0') + '\n';
        }
        return out;
    }

    friend std::ostream &operator<<(std::ostream &, const BooleanFunction &) noexcept;

private:
    std::vector<bool> v_;
};

std::ostream &operator<<(std::ostream &out, const BooleanFunction &bf) noexcept {
    for (auto v: bf.v_) {
        out << (v ? '1' : '0');
    }
    return out;
}

class Substitution {
};

#endif //QUANTUM_CIRCUIT_SYNTHESIS_PRIMITIVES_HPP
