#include "primitives.hpp"


// Boolean function
BooleanFunction::BooleanFunction(size_t n, size_t dim) {
    // create bf x_n in basis {x_0, x_1, ..., x_{n-1}}
    if (!dim) {
        throw BFException("Invalid BF dimension");
    }
    if (n > dim - 1) {
        throw BFException("Invalid number of bf variables");
    }
    size_t len = 1 << dim;
    int counter = 0;
    bool value = false;
    vec_.reserve(len);
    for (size_t i = 0; i < len; i++) {
        if (counter == 1 << (dim - n - 1)) {
            counter = 0;
            value = !value;
        }
        counter++;
        vec_.push_back(value);
    }
}

BooleanFunction::BooleanFunction(bool bit, size_t dim) {
    // create const f(x_0, x_1, ..., x_{n-1})=bit
    if (!dim) {
        throw BFException("Invalid BF dimension");
    }
    for (auto i = 0; i < 1 << dim; i++) {
        vec_.push_back(bit);
    }
}

BooleanFunction::BooleanFunction(const binary_vector &v) {
    if (v.size() == 1 || !is_power_of_2(v.size())) {
        throw BFException("Invalid BF vector length");
    }
    vec_ = v;
}

BooleanFunction::BooleanFunction(const std::vector<int> &v) {
    if (v.size() == 1 || !is_power_of_2(v.size())) {
        throw BFException("Invalid BF vector length");
    }
    for (auto i: v) {
        if (i == 1) {
            vec_.push_back(true);
        } else if (!i) {
            vec_.push_back(false);
        } else {
            throw BFException("Unexpected value if BF vector: " + std::to_string(i));
        }
    }
}

BooleanFunction::BooleanFunction(const std::string &s) {
    if (s.size() == 1 || !is_power_of_2(s.size())) {
        throw BFException("Invalid BF vector length");
    }
    for (auto i: s) {
        if (i == '1') {
            vec_.push_back(true);
        } else if (i == '0') {
            vec_.push_back(false);
        } else {
            throw BFException("Unexpected value if BF vector: " + std::to_string(i));
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
    return vec_ == bf.vec_;
}

bool BooleanFunction::operator!=(const BooleanFunction &bf) const {
    return !this->operator==(bf);
}

BooleanFunction &BooleanFunction::operator+=(const BooleanFunction &bf) {
    if (this->dim() != bf.dim()) {
        throw BFException("Boolean functions must have the same dimensions");
    }
    for (size_t i = 0; i < vec_.size(); i++) {
        vec_[i] = vec_[i] != bf.vec_[i];
    }
    return *this;
}

BooleanFunction &BooleanFunction::operator*=(const BooleanFunction &bf) {
    if (this->dim() != bf.dim()) {
        throw BFException("Boolean functions must have the same dimensions");
    }
    for (size_t i = 0; i < vec_.size(); i++) {
        vec_[i] = vec_[i] && bf.vec_[i];
    }
    return *this;
}

BooleanFunction &BooleanFunction::operator|=(const BooleanFunction &bf) {
    if (this->dim() != bf.dim()) {
        throw BFException("Boolean functions must have the same dimensions");
    }
    for (size_t i = 0; i < vec_.size(); i++) {
        vec_[i] = vec_[i] || bf.vec_[i];
    }
    return *this;
}

BooleanFunction &BooleanFunction::operator~() noexcept {
    for (auto &&bit: vec_) {
        bit = !bit;
    }
    return *this;
}

BooleanFunction::operator bool() const {
    if (!this->is_constant()) {
        throw BFException("Unable to cast BF into bool");
    }
    return vec_.front();
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

bool BooleanFunction::is_constant() const noexcept {
    return !bool(std::count_if(vec_.cbegin(), vec_.cend(), [bit = vec_.front()](bool b) {
        return bit != b;
    }));
}

size_t BooleanFunction::variable() const {
    for (size_t i = 0; i < this->dim(); i++) {
        if (this->operator==(BooleanFunction(i, this->dim()))) {
            return i;
        }
    }
    throw BFException("BF is not variable");
}

std::vector<bool> BooleanFunction::mobius_transformation() const noexcept {
    std::vector<bool> anf(vec_);

    for (size_t s = 0; s < this->dim(); ++s) {
        const size_t mask = 1 << s;
        for (size_t i = 0; i < vec_.size(); i++) {
            if (i & mask) {
                anf[i] = anf[i] ^ anf[i ^ mask];
            }
        }
    }

    return anf;
}

std::vector<int> BooleanFunction::RW_spectrum() const noexcept {
    std::vector<int> spectrum;
    spectrum.reserve(vec_.size());

    for (size_t u = 0; u < vec_.size(); u++) {
        int sum = 0;
        for (size_t x = 0; x < vec_.size(); x++) {
            sum += vec_[x] * static_cast<int>(std::pow(-1, binary_dot(u, x)));
        }
        spectrum.push_back(sum);
    }
    return spectrum;
}

int BooleanFunction::adjacent_zeros() const noexcept {
    auto spectrum = this->RW_spectrum();
    double sum = 0;

    for (size_t v = 0; v < this->size(); v++) {
        int weight = 0;
        size_t v_copy = v;
        while (v_copy) {
            weight++;
            v_copy &= v_copy - 1;
        }
        sum += weight * spectrum[v] * spectrum[v];
    }
    return static_cast<int>((this->size() * this->dim() - sum / std::pow(2, static_cast<int>(this->dim()) - 2)) / 2);
}

int BooleanFunction::complexity() const noexcept {
    auto spectrum = this->RW_spectrum();
    return this->adjacent_zeros() + this->dim() * this->size() *
                                    std::count_if(spectrum.begin(), spectrum.end(), [](int v) {
                                        return !bool(v);
                                    });
}

BinaryMapping BooleanFunction::extend() const {
    // extends a boolean function to an invertible mapping
    // the values of the original function will be achieved by feeding zeros to additional inputs
    // if f(x1,...,xn) make f1(x1,...,x{n+1})=f+x{n+1}
    // f1(x1,...,xn,0)=f
    binary_vector bf_values;
    size_t height;
    std::string truth_table_s;
    size_t width = this->dim();
    if (!this->is_balanced()) {
        height = (1 << (this->dim() + 1)) - 1;
        width += 1;
        for (auto bit: vec_) {
            bf_values.push_back(bit);
            bf_values.push_back(!bit);
        }
    } else {
        height = (1 << this->dim()) - 1;
        bf_values = vec_;
    }

    size_t even = height;
    size_t odd = height - 1;
    for (auto bit: bf_values) {
        if (bit) {
            truth_table_s += decimal_to_binary_s(height - odd, width) + '\n';
            odd -= 2;
            continue;
        }
        truth_table_s += decimal_to_binary_s(height - even, width) + '\n';
        even -= 2;
    }
    return BinaryMapping(truth_table_s);
}

binary_vector BooleanFunction::vector() const noexcept {
    return vec_;
}

std::string BooleanFunction::to_table(char sep) const noexcept {
    std::string out;
    std::string set;
    for (size_t i = 0; i < this->size(); i++) {
        out += decimal_to_binary_s(i, this->dim()) + sep + (vec_[i] ? '1' : '0') + '\n';
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

BooleanFunction operator*(const BooleanFunction &bf1, const BooleanFunction &bf2) {
    BooleanFunction bf3(bf1);
    bf3 *= bf2;
    return bf3;
}

BooleanFunction operator|(const BooleanFunction &bf1, const BooleanFunction &bf2) {
    BooleanFunction bf3(bf1);
    bf3 |= bf2;
    return bf3;
}

// Binary mapping

BinaryMapping::BinaryMapping(const cf_set &t) {
    if (t.empty()) {
        throw BMException("Empty set of coordinate functions");
    }
    auto len = t.front().size();
    for (const auto &bf: t) {
        if (bf.size() != len) {
            cf_.clear();
            throw BMException("Coordinate boolean functions must have the same length");
        }
        cf_.push_back(bf);
    }
}

BinaryMapping::BinaryMapping(const table &t) {
    if (t.empty()) {
        throw BMException("Empty truth table");
    }
    auto len = t.front().size();
    for (const auto &vec: t) {
        if (vec.size() != len) {
            cf_.clear();
            throw BMException("Coordinate boolean functions must have the same length");
        }
        cf_.emplace_back(vec);
    }
}

BinaryMapping::BinaryMapping(const std::string &s) {
    by_string_(s);
}

BinaryMapping::BinaryMapping(std::istream &s) {
    std::stringstream ss;
    ss << s.rdbuf();
    by_string_(ss.str());
}

BinaryMapping::BinaryMapping(const BinaryMapping &mp) {
    cf_ = mp.cf_;
}

BinaryMapping::BinaryMapping(const Substitution &sub) {
    auto power = static_cast<size_t>(std::log2(sub.power()));
    table truth_table(power);
    for (const auto &v: sub.vector()) {
        auto v_binary = decimal_to_binary_s(v, power);
        for (size_t i = 0; i < v_binary.size(); i++) {
            truth_table[i].push_back(v_binary[i] == '1');
        }
    }
    cf_.reserve(truth_table.size());
    for (const auto &bf: truth_table) {
        cf_.emplace_back(bf);
    }
}

BinaryMapping &BinaryMapping::operator=(const BinaryMapping &mp) {
    if (this->operator!=(mp)) {
        cf_.clear();
        cf_ = mp.cf_;
    }
    return *this;
}

BinaryMapping &BinaryMapping::operator=(const Substitution &sub) {
    BinaryMapping mp(sub);
    if (this->operator!=(mp)) {
        cf_.clear();
        cf_ = mp.cf_;
    }
    return *this;
}

bool BinaryMapping::operator==(const BinaryMapping &mp) const {
    return cf_ == mp.cf_;
}

bool BinaryMapping::operator!=(const BinaryMapping &mp) const {
    return !this->operator==(mp);
}

bool BinaryMapping::operator==(const Substitution &sub) const {
    return this->operator==(BinaryMapping(sub));
}

bool BinaryMapping::operator!=(const Substitution &sub) const {
    return !this->operator==(sub);
}

cf_set BinaryMapping::coordinate_functions() const noexcept {
    return cf_;
}

size_t BinaryMapping::inputs_number() const noexcept {
    return cf_.front().dim();
}

size_t BinaryMapping::outputs_number() const noexcept {
    return cf_.size();
}

bool BinaryMapping::is_substitution() const noexcept {
    try {
        auto _ = Substitution(*this);
        return true;
    } catch (...) {
        return false;
    }
}

BinaryMapping BinaryMapping::extend() const {
    // extends the mapping to a reversible mapping
    if (cf_.size() == 1) {
        return BooleanFunction(cf_.front()).extend();
    }

    if (this->is_substitution()) {
        return *this;
    }

    int inputs = cf_.front().dim();
    int outputs = cf_.size();
    table truth_table;

    for (int i = inputs + outputs - 1; i >= 0; i--) {
        auto column_bf = BooleanFunction(static_cast<size_t>(i), outputs + inputs);
        if (i - inputs < 0) {
            truth_table.insert(truth_table.begin(), column_bf.vector());
            continue;
        }
        binary_vector extension;
        for (auto bit: cf_[i - inputs].vector()) {
            for (auto _ = 0; _ < 1 << outputs; _++) {
                extension.push_back(bit);
            }
        }
        truth_table.insert(truth_table.begin(), (BooleanFunction(extension) + column_bf).vector());
    }
    return BinaryMapping(truth_table);
}

table BinaryMapping::to_table_() const noexcept {
    table result;
    for (const auto &bf: cf_) {
        result.push_back(bf.vector());
    }
    return result;
}

void BinaryMapping::by_string_(const std::string &s) {
    if (s.empty()) {
        throw BMException("Empty truth table");
    }
    std::stringstream ss(s);
    std::string line;
    table truth_table;

    while (getline(ss, line, '\n')) {
        trim(line);
        if (line.empty() || line.front() == '#') {
            continue;
        }
        size_t current_col = 0;
        for (char i: line) {
            if (isspace(i)) {
                continue;
            }
            if (i == '0') {
                if (current_col >= truth_table.size()) {
                    truth_table.emplace_back();
                }
                truth_table[current_col].push_back(false);
                current_col++;
            } else if (i == '1') {
                if (current_col >= truth_table.size()) {
                    truth_table.emplace_back();
                }
                truth_table[current_col].push_back(true);
                current_col++;
            } else {
                throw BMException(("Unexpected symbol in truth table: ") + i);
            }
        }
    }
    if (!std::all_of(truth_table.begin(), truth_table.end(),
                     [first_size = truth_table[0].size()](const auto &v) {
                         return v.size() == first_size;
                     })) {
        throw BMException("Coordinate boolean functions must have the same length");
    }
    for (const auto &vec: truth_table) {
        cf_.emplace_back(vec);
    }
}

std::string BinaryMapping::to_table(char sep) const noexcept {
    auto truth_table = this->to_table_();
    std::string result;
    for (size_t i = 0; i < truth_table.front().size(); i++) {
        result += decimal_to_binary_s(i, this->inputs_number()) + sep;
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

bool is_substitution(const std::vector<size_t> &vec) {
    if (vec.size() < 2) {
        return false;
    }
    std::unordered_set<size_t> checked;
    for (auto v: vec) {
        if (v >= vec.size() || checked.count(v)) {
            return false;
        }
        checked.insert(v);
    }
    return checked.size() == vec.size();
}

size_t cayley_distance(const Substitution &sub1, const Substitution &sub2) {
    // sub1 = g; sub2 = h
    int n = std::max(sub1.power(), sub2.power());
    auto hg = sub2.invert() * sub1;
    return n - hg.cycles().size();
}

Substitution::Substitution(const std::vector<size_t> &v) {
    if (v.empty()) {
        throw SubException("Empty images set");
    }
    sub_ = v;
    if (!is_substitution(sub_)) {
        sub_.clear();
        throw SubException("Unable to build substitution");
    }
}

Substitution::Substitution(const std::vector<cycle_type> &cycles) {
    *this = Substitution(2);
    for (const auto &cycle: cycles) {
        *this *= substitution_by_cycle(cycle);
    }
}

Substitution::Substitution(const std::vector<transposition_type> &transpositions) {
    if (transpositions.empty()) {
        throw SubException("Unable to handle empty transpositions set");
    }
    size_t power = 0;
    for (const auto &trans: transpositions) {
        power = trans.first > power ? trans.first : power;
        power = trans.second > power ? trans.second : power;
    }

    *this = Substitution(power + 1);
    for (const auto &[i, j]: transpositions) {
        auto temp_sub = Substitution(power + 1);
        std::swap(temp_sub.sub_[i], temp_sub.sub_[j]);
//        *this = *this * temp_sub;
        *this = temp_sub * *this;
    }

    if (!is_substitution(sub_)) {
        sub_.clear();
        throw SubException("Unable to build substitution");
    }
}

Substitution::Substitution(const cf_set &cf) {
    if (cf.empty()) {
        throw SubException("Empty coordinate function set");
    }

    auto bf_dim = cf.front().dim();
    auto bf_size = cf.front().size();

    if (!std::all_of(cf.begin(), cf.end(),
                     [bf_dim](const auto &v) {
                         return v.dim() == bf_dim;
                     })) {
        throw SubException("Coordinate boolean functions must have the same length");
    }
    if (bf_dim != cf.size()) {
        throw SubException("Coordinate boolean functions form an irreversible mapping");
    }
    table truth_table(bf_dim);
    std::transform(cf.begin(), cf.end(), truth_table.begin(),
                   [](const auto &bf) {
                       return bf.vector();
                   });
    sub_ = std::vector<size_t>(bf_size);
    for (size_t i = 0; i < bf_size; i++) {
        std::string line;
        for (const auto &v: truth_table) {
            line += v[i] ? '1' : '0';
        }
        sub_[i] = binary_to_decimal(line);
    }
    if (!is_substitution(sub_)) {
        sub_.clear();
        throw SubException("Unable to build substitution");
    }
}

Substitution::Substitution(const table &t) {
    if (t.empty()) {
        throw SubException("Empty truth table");
    }

    auto col_size = t.front().size();
    if (!std::all_of(t.begin(), t.end(),
                     [col_size](const auto &col) {
                         return col.size() == col_size;
                     })) {
        throw SubException("Coordinate boolean functions must have the same length");
    }
    if (static_cast<size_t>(std::log2(col_size)) != t.size()) {
        throw SubException("Coordinate boolean functions form an irreversible mapping");
    }
    sub_ = std::vector<size_t>(col_size);
    for (size_t i = 0; i < col_size; i++) {
        std::string line;
        for (const auto &v: t) {
            line += v[i] ? '1' : '0';
        }
        sub_.push_back(binary_to_decimal(line));
    }
    if (!is_substitution(sub_)) {
        sub_.clear();
        throw SubException("Unable to build substitution");
    }
}

Substitution::Substitution(const std::string &s) {
    by_string_(s);
}

Substitution::Substitution(const size_t power) {
    if (power < 2) {
        throw SubException("Substitution power should be greater than 1");
    }
    for (size_t i = 0; i < power; i++) {
        sub_.push_back(i);
    }
}

Substitution::Substitution(std::istream &s) {
    std::stringstream ss;
    ss << s.rdbuf();
    by_string_(ss.str());
}

Substitution::Substitution(const Substitution &sub) {
    sub_ = sub.sub_;
}

Substitution::Substitution(const BinaryMapping &mp) {
    sub_ = Substitution(mp.coordinate_functions()).sub_;
}

Substitution &Substitution::operator=(const Substitution &sub) {
    if (this->operator!=(sub)) {
        sub_.clear();
        sub_ = sub.sub_;
    }
    return *this;
}

Substitution &Substitution::operator=(const BinaryMapping &mp) {
    Substitution sub(mp);
    if (this->operator!=(sub)) {
        sub_.clear();
        sub_ = sub.sub_;
    }
    return *this;
}

bool Substitution::operator==(const Substitution &sub) const {
    return sub_ == sub.sub_;
}

bool Substitution::operator!=(const Substitution &sub) const {
    return !this->operator==(sub);
}

bool Substitution::operator==(const BinaryMapping &bm) const {
    try {
        return this->operator==(Substitution(bm));
    } catch (...) {
        return false;
    }
}

bool Substitution::operator!=(const BinaryMapping &bm) const {
    return !this->operator==(bm);
}

Substitution &Substitution::operator*=(const Substitution &s) {
    std::vector<size_t> images(std::max(s.power(), power()));

    for (size_t i = 0; i < images.size(); i++) {
        if (i < sub_.size()) {
            auto image = sub_[i];
            if (image >= s.sub_.size()) {
                images[i] = image;
                continue;
            }
            images[i] = s.sub_[image];
            continue;
        }
        images[i] = s.sub_[i];
    }

    sub_ = images;
    return *this;
}

size_t Substitution::power() const noexcept {
    return sub_.size();
}

bool Substitution::is_identical() const noexcept {
    for (size_t i = 0; i < sub_.size(); i++) {
        if (sub_[i] != i) {
            return false;
        }
    }
    return true;
}

std::vector<size_t> Substitution::vector() const noexcept {
    return sub_;
}

std::vector<cycle_type> Substitution::cycles() const noexcept {
    std::unordered_set<size_t> visited;
    std::vector<cycle_type> cycles;

    for (size_t i = 0; i < sub_.size(); i++) {
        if (visited.count(i)) {
            continue;
        }

        cycle_type cycle;
        auto element = i;
        while (!visited.count(element)) {
            visited.insert(element);
            cycle.push_back(element);
            element = sub_[element];
        }
        cycles.push_back(cycle);
    }
    return cycles;
}

std::vector<transposition_type> Substitution::transpositions() const noexcept {
    std::vector<transposition_type> transpositions;
    for (const auto &cycle: this->cycles()) {
        if (cycle.size() == 1) {
            continue;
        }
        for (size_t i = 0; i < cycle.size() - 1; i++) {
//            transpositions.insert(transpositions.begin(), std::make_pair(cycle[i], cycle[i + 1]));
            transpositions.emplace_back(cycle[i], cycle[i + 1]);
        }
    }
    return transpositions;
}

std::vector<cycle_type> Substitution::tcycles() const noexcept {
    // tcycles = 3-cycles
    std::vector<cycle_type> result;
    std::vector<cycle_type> two_cycles;
    for (const auto &cycle: this->cycles()) {
        if (cycle.size() < 4) {
            if (cycle.size() == 2) {
                two_cycles.push_back(cycle);
                continue;
            }
            result.push_back(cycle);
            continue;
        }
        size_t i = 1;
        for (; i < cycle.size() - 2;) {
            result.emplace_back();
            result.back().reserve(3);
            result.back().push_back(cycle[0]);
            result.back().push_back(cycle[i]);
            result.back().push_back(cycle[++i]);
            ++i;
        }
        if (i < cycle.size()) {
            cycle_type current_cycle;
            current_cycle.reserve(3);
            current_cycle.push_back(cycle[0]);
            for (; i < cycle.size(); i++) {
                current_cycle.push_back(cycle[i]);
            }
            if (current_cycle.size() == 2) {
                two_cycles.push_back(current_cycle);
                continue;
            }
            result.push_back(current_cycle);
        }
    }

    if (two_cycles.size() < 2) {
        std::copy(two_cycles.begin(), two_cycles.end(), std::back_inserter(result));
        return result;
    }
    for (size_t i = 0; i < two_cycles.size() - 1; i += 2) {
        auto &left = two_cycles[i];
        auto &right = two_cycles[i + 1];

        left = {left.front(), right.front(), left.back()};
        std::rotate(
                left.begin(),
                left.begin() + std::distance(
                        left.begin(),
                        std::min_element(left.begin(), left.end())
                ),
                left.end()
        );

        right = {left.back(), right.back(), right.front()};
        std::rotate(
                right.begin(),
                right.begin() + std::distance(
                        right.begin(),
                        std::min_element(right.begin(), right.end())
                ),
                right.end()
        );
    }
    std::copy(two_cycles.begin(), two_cycles.end(), std::back_inserter(result));

    return result;
}

size_t heterogeneous_columns_number(const cycle_type &cycle) {
    if (cycle.size() != 3) {
        throw SubException("Cycle should have length of 3");
    }
    auto max_value = *std::max_element(cycle.begin(), cycle.end());
    size_t max_length = std::bit_width(max_value);
    size_t count = 0;

    for (size_t i = 0; i < max_length; i++) {
        auto bit_pos = max_length - i - 1;
//        auto u_bit = (cycle[0] >> bit_pos) & 1;
//        auto s_bit = (cycle[1] >> bit_pos) & 1;
//        auto t_bit = (cycle[2] >> bit_pos) & 1;
//        count += !(u_bit == s_bit && s_bit == t_bit);
        count += !((((cycle[0] >> bit_pos) & 1) + ((cycle[1] >> bit_pos) & 1) + ((cycle[2] >> bit_pos) & 1)) % 3);
    }

    return count;
}

std::vector<cycle_type> Substitution::ncycles() const noexcept {
    // ncycles - neighbor cycles
    for (const auto &cycle: tcycles()) {
        auto het_cols = heterogeneous_columns_number(cycle);
        if (het_cols == 1) {
            continue;
        }
    }
    return {};
}

Substitution Substitution::invert() const noexcept {
    std::vector<size_t> images(sub_.size());
    for (size_t i = 0; i < sub_.size(); i++) {
        images[sub_[i]] = i;
    }
    return Substitution(images);
}

bool Substitution::is_odd() const noexcept {
    int indicator = 1;  // is not odd
    for (const auto &cycle: this->cycles()) {
        indicator *= static_cast<int>(std::pow(-1, cycle.size() - 1));
    }
    return indicator == -1;
}

void Substitution::by_string_(const std::string &s) {
    if (s.empty()) {
        throw SubException("Empty substitution");
    }
    std::stringstream ss(s);
    std::string line;
    std::string value;

    while (getline(ss, line, '\n')) {
        trim(line);
        if (line.empty() || line.front() == '#') {
            continue;
        }
        std::stringstream line_ss(line);
        while (line_ss >> value) {
            size_t value_int = 0;
            if (!try_string_to_decimal(value, value_int)) {
                throw SubException("Invalid value: " + value);
            }
            sub_.push_back(value_int);
        }
    }
    if (!is_substitution(sub_)) {
        sub_.clear();
        throw SubException("Unable to build substitution");
    }
}

Substitution substitution_by_cycle(const cycle_type &cycle) {
    if (cycle.empty()) {
        throw SubException("Unable to handle empty cycle");
    }
    size_t power = *std::max_element(cycle.begin(), cycle.end());
    if (!power) {
        return Substitution(2);
    }
    Substitution sub(power + 1);
    if (cycle.size() == 1) {
        return sub;
    }
    for (size_t i = 0; i < cycle.size() - 1; i++) {
        std::swap(sub.sub_[cycle[i]], sub.sub_[cycle[i + 1]]);
    }
    return sub;
}

std::ostream &operator<<(std::ostream &out, const Substitution &sub) noexcept {
    for (size_t i = 0; i < sub.power(); i++) {
        out << sub.sub_[i] << ' ';
    }
    return out;
}

Substitution operator*(const Substitution &sub1, const Substitution &sub2) {
    Substitution sub3(sub1);
    sub3 *= sub2;
    return sub3;
}
