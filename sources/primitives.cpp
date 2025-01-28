#include "primitives.hpp"

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


// Boolean function

BooleanFunction::BooleanFunction(const binary_vector &v) {
    if (!is_power_of_2(v.size())) {
        throw std::runtime_error{"Invalid length"};
    }
    vec_ = v;
}

BooleanFunction::BooleanFunction(const std::vector<int> &v) {
    if (!is_power_of_2(v.size())) {
        throw std::runtime_error{"Invalid length"};
    }
    for (auto i: v) {
        if (i == 1) {
            vec_.push_back(true);
        } else if (!i) {
            vec_.push_back(false);
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
            vec_.push_back(true);
        } else if (i == '0') {
            vec_.push_back(false);
        } else {
            throw std::runtime_error{"Unexpected value"};
        }
    }
}

BooleanFunction::BooleanFunction(const BooleanFunction &bf) {
    vec_ = bf.vec_;
}

BooleanFunction &BooleanFunction::operator=(const BooleanFunction &bf) {
    if (this->operator!=(bf)) {
        vec_.clear();
        vec_ = bf.vec_;
    }
    return *this;
}

bool BooleanFunction::operator==(const BooleanFunction &bf) const {
    if (this->size() != bf.size()) {
        return false;
    }
    for (size_t i = 0; i < this->size(); i++) {
        if (vec_[i] != bf.vec_[i]) {
            return false;
        }
    }
    return true;
}

bool BooleanFunction::operator!=(const BooleanFunction &bf) const {
    return !this->operator==(bf);
}

BooleanFunction &BooleanFunction::operator+=(const BooleanFunction &bf) {
    if (this->dim() != bf.dim()) {
        throw std::runtime_error{"Boolean functions must to be the same dimensions"};
    }
    for (size_t i = 0; i < vec_.size(); i++) {
        vec_[i] = vec_[i] != bf.vec_[i];
    }
    return *this;
}

size_t BooleanFunction::size() const noexcept {
    return vec_.size();
}

size_t BooleanFunction::dim() const noexcept {
    return static_cast<size_t>(std::log2(this->size()));
}

size_t BooleanFunction::weight() const noexcept {
    return std::accumulate(vec_.begin(), vec_.end(), 0);
}

bool BooleanFunction::is_balanced() const noexcept {
    return this->weight() * 2 == this->size();
}

binary_vector BooleanFunction::get_vector() const noexcept {
    return vec_;
}

std::string BooleanFunction::to_table(char sep) const noexcept {
    std::string out;
    std::string set;
    for (size_t i = 0; i < this->size(); i++) {
        out += decimal_to_binary(i, this->dim()) + sep + (vec_[i] ? '1' : '0') + '\n';
    }
    return out;
}

std::ostream &operator<<(std::ostream &out, const BooleanFunction &bf) noexcept {
    for (auto v: bf.vec_) {
        out << (v ? '1' : '0');
    }
    return out;
}

BooleanFunction operator+(const BooleanFunction &bf1, const BooleanFunction &bf2) {
    BooleanFunction bf3(bf1);
    bf3 += bf2;
    return bf3;
}


// Binary mapping

BinaryMapping::BinaryMapping(const cf_set &t) {
    if (t.empty()) {
        throw std::runtime_error{"Empty set of coordinate functions"};
    }
    auto len = t.front().size();
    if (len == 1) {
        throw std::runtime_error{"Coordinate function could not be a constant"};
    }
    for (const auto &bf: t) {
        if (bf.size() != len) {
            cf_.clear();
            throw std::runtime_error{"Coordinate boolean functions should have the same length"};
        }
        cf_.push_back(bf);
    }
}

BinaryMapping::BinaryMapping(const table &t) {
    if (t.empty()) {
        throw std::runtime_error{"Empty table"};
    }
    auto len = t.front().size();
    if (len == 1) {
        throw std::runtime_error{"Coordinate function could not be a constant"};
    }
    for (const auto &vec: t) {
        if (vec.size() != len) {
            cf_.clear();
            throw std::runtime_error{"Coordinate boolean functions should have the same length"};
        }
        cf_.emplace_back(vec);
    }
}

BinaryMapping::BinaryMapping(const std::string &s) {
    if (s.empty()) {
        throw std::runtime_error{"Empty table"};
    }
    table truth_table;
    std::istringstream stream(s);
    std::string line;
    while (getline(stream, line)) {
        if (line.empty()) {
            throw std::runtime_error{"Empty line in tables are not allowed"};
        }
        if (truth_table.size() != line.size()) {
            if (!truth_table.empty()) {
                throw std::runtime_error{"Table lines should have equal length"};
            } else {
                for (size_t i = 0; i < line.size(); i++) {
                    truth_table.emplace_back();
                }
            }
        }
        for (size_t i = 0; i < line.size(); i++) {
            if (line[i] == '0') {
                truth_table[i].push_back(false);
            } else if (line[i] == '1') {
                truth_table[i].push_back(true);
            } else {
                throw std::runtime_error{"Unexpected symbol"};
            }
        }
    }
    for (const auto &vec: truth_table) {
        cf_.emplace_back(vec);
    }
}

//BinaryMapping::BinaryMapping(const Substitution &sub) {
//
//}

BinaryMapping::BinaryMapping(const BinaryMapping &mp) {
    cf_ = mp.cf_;
}

BinaryMapping &BinaryMapping::operator=(const BinaryMapping &mp) {
    if (this->operator!=(mp)) {
        cf_.clear();
        cf_ = mp.cf_;
    }
    return *this;
}

//BinaryMapping &BinaryMapping::operator=(const Substitution &sub) {
//}

bool BinaryMapping::operator==(const BinaryMapping &mp) const {
    if (this->get_inputs_number() != mp.get_inputs_number() || this->get_outputs_number() != mp.get_outputs_number()) {
        return false;
    }
    for (size_t i = 0; i < cf_.size(); i++) {
        if (cf_[i] != mp.cf_[i]) {
            return false;
        }
    }
    return true;
}

bool BinaryMapping::operator!=(const BinaryMapping &mp) const {
    return !this->operator==(mp);
}

cf_set BinaryMapping::get_coordinate_functions() const noexcept {
    return cf_;
}

bool BinaryMapping::is_substitution() const noexcept {
    if (this->get_inputs_number() != this->get_outputs_number()) {
        return false;
    }
//    if (std::any_of(cf_.begin(), cf_.end(),
//                    [&](const BooleanFunction &bf) {
//                        return !bf.is_balanced();
//                    })) {
//        return false;
//    }

    // ...

    return true;
}

size_t BinaryMapping::get_inputs_number() const noexcept {
    return cf_.front().dim();
}

size_t BinaryMapping::get_outputs_number() const noexcept {
    return cf_.size();
}

table BinaryMapping::to_table_() const noexcept {
    table result;
    for (const auto &bf: cf_) {
        result.push_back(bf.get_vector());
    }
    return result;
}

std::string BinaryMapping::to_table(char sep) const noexcept {
    auto truth_table = this->to_table_();
    std::string result;
    for (size_t i = 0; i < truth_table.front().size(); i++) {
        result += decimal_to_binary(i, this->get_inputs_number()) + sep;
        for (auto &j: truth_table) {
            result += (j[i] ? '1' : '0');
        }
        result += '\n';
    }
    return result;
}

std::ostream &operator<<(std::ostream &out, const BinaryMapping &mp) noexcept {
    auto truth_table = mp.to_table_();
    for (size_t i = 0; i < truth_table.front().size(); i++) {
        for (auto &j: truth_table) {
            out << (j[i] ? '1' : '0');
        }
        out << '\n';
    }
    return out;
}


// Substitution

Substitution::Substitution(const cf_set &cf) : BinaryMapping(cf) {
    if (this->get_inputs_number() != this->get_outputs_number()) {
        throw std::runtime_error{"This mapping is not substitution"};
    }
    auto truth_table = this->to_table_();
    auto power = static_cast<size_t>(std::pow(2, this->get_inputs_number()));
    sub_ = std::vector<size_t>(power);
    binary_vector indicator(power, false);
    std::string line;
    size_t value = 0;
    for (size_t i = 0; i < truth_table.front().size(); i++) {
        for (auto &j: truth_table) {
            line += (j[i] ? '1' : '0');
        }
        value = binary_to_decimal(line);
        if (indicator[value]) {
            throw std::runtime_error{"This mapping is not substitution"};
        }
        indicator[value] = true;
        sub_[i] = value;
        line.clear();
    }
}

Substitution::Substitution(const table &t) : BinaryMapping(t) {

}

Substitution::Substitution(const std::string &s) : BinaryMapping(s) {

}

//Substitution::Substitution(const Substitution &sub) : BinaryMapping(sub) {
//
//}

//Substitution &Substitution::operator=(const Substitution &sub) {
//}

//Substitution &Substitution::operator=(const BinaryMapping &) {
//
//}

size_t Substitution::power() const noexcept {
    return sub_.size();
}

std::vector<size_t> Substitution::get_vector() const noexcept {
    return sub_;
}

bool Substitution::is_odd() const noexcept {
    return true;
}

std::ostream &operator<<(std::ostream &out, const Substitution &sub) noexcept {
    for (size_t i = 0; i < sub.power(); i++) {
        out << i << ' ';
    }
    out << '\n';
    for (auto v: sub.sub_) {
        out << v << ' ';
    }
    out << '\n';
    return out;
}
