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
            truth_table_s += decimal_to_binary(height - odd, width) + '\n';
            odd -= 2;
            continue;
        }
        truth_table_s += decimal_to_binary(height - even, width) + '\n';
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
        auto v_binary = decimal_to_binary(v, power);
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
    if (this->inputs_number() != mp.inputs_number() || this->outputs_number() != mp.outputs_number()) {
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
        result += decimal_to_binary(i, this->inputs_number()) + sep;
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
    if (vec.empty()) {
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

std::vector<std::pair<size_t, size_t>> Substitution::transpositions() const noexcept {
    std::vector<std::pair<size_t, size_t>> transpositions;
    for (const auto &cycle: this->cycles()) {
        if (cycle.size() == 1) {
            continue;
        }
        for (size_t i = 0; i < cycle.size() - 1; i++) {
            transpositions.emplace_back(cycle[i], cycle[i + 1]);
        }
    }
    return transpositions;
}

std::vector<std::vector<size_t>> Substitution::cycles() const noexcept {
    std::unordered_set<size_t> visited;
    std::vector<std::vector<size_t>> cycles;

    for (size_t i = 0; i < sub_.size(); i++) {
        if (visited.count(i)) {
            continue;
        }

        std::vector<size_t> cycle;
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

std::ostream &operator<<(std::ostream &out, const Substitution &sub) noexcept {
    for (size_t i = 0; i < sub.power(); i++) {
        out << sub.sub_[i] << ' ';
    }
    return out;
}
