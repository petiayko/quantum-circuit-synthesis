#include "primitives.hpp"

//bool is_substitution(const substitution &sub) {
//    return sub.empty();
//}

//substitution table_to_substitution(const binary_substitution &t) {
//    auto power = t.size();
//    if (!is_power_of_2(power)) {
//        throw std::runtime_error{""};
//    }
//    for (auto const &vec: t) {
//        if (power != vec.size()) {
//            throw std::runtime_error{""};
//        }
//    }
//
//    substitution result;
//    std::string binary_element;
//    for (size_t i = 0; i < power; i++) {
//        for (size_t j = 0; j < static_cast<size_t>(std::log2(power)); j++) {
//            binary_element += (t[i][j] ? '1' : '0');
//        }
//        result.push_back(binary_to_decimal<size_t>(binary_element));
//        binary_element.clear();
//    }
//    return result;
//}

//binary_substitution substitution_to_table(const substitution &sub) {
//    auto power = sub.size();
//    if (!is_power_of_2(power)) {
//        throw std::runtime_error{"Incorrect power of substitution. Should be power of 2"};
//    }
//    if (!is_substitution(sub)) {
//        throw std::runtime_error{"Incorrect substitution"};
//    }
//
//    auto power_log = static_cast<size_t>(std::log2(power));
//    binary_substitution result(power_log);
//    std::string binary_element;
//    for (auto element: sub) {
//        binary_element = decimal_to_binary(element, power_log);
//        for (size_t i = 0; i < power_log; i++) {
//            result[i].push_back(bool(int(binary_element[i])));
//        }
//    }
//    return result;
//}

BooleanFunction::BooleanFunction(const std::vector<bool> &v) {
    if (!is_power_of_2(v.size())) {
        throw std::runtime_error{"Invalid length"};
    }
    v_ = v;
}

BooleanFunction::BooleanFunction(const std::vector<int> &v) {
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

BooleanFunction::BooleanFunction(const std::string &s) {
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

BooleanFunction::BooleanFunction(const BooleanFunction &bf) {
    v_ = bf.v_;
}

BooleanFunction &BooleanFunction::operator=(const BooleanFunction &bf) {
    if (this->operator!=(bf)) {
        v_.clear();
        v_ = bf.v_;
    }
    return *this;
}

bool BooleanFunction::operator==(const BooleanFunction &bf) const {
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

bool BooleanFunction::operator!=(const BooleanFunction &bf) const {
    return !this->operator==(bf);
}

size_t BooleanFunction::size() const noexcept {
    return v_.size();
}

size_t BooleanFunction::dim() const noexcept {
    return static_cast<size_t>(std::log2(this->size()));
}

size_t BooleanFunction::weight() const noexcept {
    return std::accumulate(v_.begin(), v_.end(), 0);
}

bool BooleanFunction::is_balanced() const noexcept {
    return this->weight() * 2 == this->size();
}

std::vector<bool> BooleanFunction::get_vector() const noexcept {
    return v_;
}

std::string BooleanFunction::to_table(char sep) const noexcept {
    std::string out;
    std::string set;
    for (size_t i = 0; i < this->size(); i++) {
        out += decimal_to_binary(i, this->dim()) + sep + (v_[i] ? '1' : '0') + '\n';
    }
    return out;
}

std::ostream &operator<<(std::ostream &out, const BooleanFunction &bf) noexcept {
    for (auto v: bf.v_) {
        out << (v ? '1' : '0');
    }
    return out;
}

Mapping::Mapping(const table &t) {
    if (t.empty()) {
        throw std::runtime_error{"Empty table"};
    }
    auto len = t.front().size();
    if (!is_power_of_2(len)) {
        throw std::runtime_error{"Invalid size of coordinate function"};
    }
    for (const auto &vec: t) {
        if (vec.size() != len) {
            table_.clear();
            throw std::runtime_error{"Coordinate boolean functions should have the same length"};
        }
        table_.push_back(vec);
    }
}

Mapping::Mapping(const std::vector<BooleanFunction> &bf_vec) {
    if (bf_vec.empty()) {
        throw std::runtime_error{"Empty set of coordinate functions"};
    }
    auto len = bf_vec.front().size();
    for (const auto &bf: bf_vec) {
        if (bf.size() != len) {
            table_.clear();
            throw std::runtime_error{"Coordinate boolean functions should have the same length"};
        }
        table_.push_back(bf.get_vector());
    }
}

//Mapping::Mapping(const std::string &s) {
//
//}

Mapping::Mapping(const Mapping &sub) {
    table_ = sub.table_;
}

std::vector<BooleanFunction> Mapping::get_coordinate_functions() const noexcept {
    std::vector<BooleanFunction> result;
    for (auto const &vec: table_) {
        result.emplace_back(vec);
    }
    return result;
}

bool Mapping::is_substitution() const noexcept {
    return false;
}

size_t Mapping::get_inputs_number() const noexcept {
    return static_cast<size_t>(std::log2(table_.front().size()));
}

size_t Mapping::get_outputs_number() const noexcept {
    return table_.size();
}
