#include "gates.hpp"


Gate::Gate(GateType type, const std::vector<size_t> &nests, const std::vector<control> &controls, size_t dim) {
    init_(type, nests, controls, dim);
}

Gate::Gate(const std::string &s, size_t dim) {
    // NAME(0, 1; 2, 3)
    if (s.empty()) {
        throw GateException("Empty string");
    }
    std::string s_copy(s);
    to_lower(s_copy);
    trim(s_copy);

    auto open_bracket_pos = s_copy.find('(');
    if (open_bracket_pos == std::string::npos) {
        throw GateException("Invalid gate format");
    }
    auto gate_name = s_copy.substr(0, open_bracket_pos);
    trim(gate_name);
    s_copy = s_copy.substr(open_bracket_pos + 1);

    auto close_bracket_pos = s_copy.find(')');
    if (s_copy.empty()) {
        throw GateException("Invalid gate format");
    }
    auto gate_params = s_copy.substr(0, close_bracket_pos);
    trim(gate_params);
    s_copy = s_copy.substr(close_bracket_pos + 1);

    if (!s_copy.empty()) {
        throw GateException("Invalid gate format");
    }

    auto semicolumn_pos = gate_params.find(';');
    std::string nests_line;
    std::string controls_line;
    if (semicolumn_pos != std::string::npos) {
        nests_line = gate_params.substr(0, semicolumn_pos);
        controls_line = gate_params.substr(semicolumn_pos + 1);
    } else {
        nests_line = gate_params;
    }

    GateType type;
    if (gate_name == "not") {
        type = GateType::NOT;
        if (semicolumn_pos != std::string::npos) {
            throw GateException("Invalid NOT gate format");
        }
    } else if (gate_name == "cnot") {
        type = GateType::CNOT;
        if (semicolumn_pos == std::string::npos) {
            throw GateException("Invalid CNOT gate format");
        }
    } else if (gate_name == "kcnot") {
        type = GateType::kCNOT;
        if (semicolumn_pos == std::string::npos) {
            throw GateException("Invalid kCNOT gate format");
        }
    } else if (gate_name == "swap") {
        type = GateType::SWAP;
        if (semicolumn_pos != std::string::npos) {
            throw GateException("Invalid SWAP gate format");
        }
    } else if (gate_name == "cswap") {
        type = GateType::CSWAP;
        if (semicolumn_pos == std::string::npos) {
            throw GateException("Invalid CSWAP gate format");
        }
    } else {
        throw GateException("Unknown gate type: " + gate_name);
    }

    // TODO make it separate function
    std::stringstream ss;
    ss << controls_line;

    std::vector<control> controls;
    std::string num_s;
    size_t num;
    bool is_direct = false;

    while (getline(ss, num_s, ',')) {
        trim(num_s);
        if (num_s.empty()) {
            continue;
        }
        if (num_s.front() == '!') {
            is_direct = false;
            num_s.erase(0, 1);
        } else {
            is_direct = true;
        }
        if (!try_string_to_decimal<size_t>(num_s, num)) {
            throw StringException("Invalid num: " + num_s);
        }
        controls.emplace_back(num, is_direct);
    }

    init_(type, string_to_num_vector(nests_line, ','), controls, dim);
}

size_t Gate::dim() const noexcept {
    return dim_;
}

GateType Gate::type() const noexcept {
    return type_;
}

std::vector<size_t> Gate::nests() const noexcept {
    return nests_;
}

std::vector<size_t> Gate::controls() const noexcept {
    std::vector<size_t> controls;
    for (const auto &[num, is_direct]: controls_) {
        controls.push_back(num);
    }
    return controls;
}

std::vector<size_t> Gate::direct_controls() const noexcept {
    std::vector<size_t> controls;
    for (const auto &[num, is_direct]: controls_) {
        if (!is_direct) {
            continue;
        }
        controls.push_back(num);
    }
    return controls;
}

std::vector<size_t> Gate::inverted_controls() const noexcept {
    std::vector<size_t> controls;
    for (const auto &[num, is_direct]: controls_) {
        if (is_direct) {
            continue;
        }
        controls.push_back(num);
    }
    return controls;
}

bool Gate::is_commutes(const Gate &gate) const {
    // TODO make it private??

    if (dim_ != gate.dim_) {
        throw GateException("Impossible to determine the commutability for gates of different dimensions");
    }

    if (gate.nests_ == nests_ || this->operator==(gate)) {
        return true;
    }

    // this - G1, gate - G2

    if (type_ != GateType::SWAP && gate.type_ != GateType::SWAP && type_ != GateType::CSWAP &&
        gate.type_ != GateType::CSWAP) {
        // first criteria
        auto t1 = nests_.front();
        auto t2 = gate.nests_.front();
        if (std::find_if(controls_.begin(), controls_.end(),
                         [t2](const auto &p) { return p.first == t2; }) == controls_.end() &&
            std::find_if(gate.controls_.begin(), gate.controls_.end(),
                         [t1](const auto &p) { return p.first == t1; }) == gate.controls_.end()) {
            return true;
        }

        // second criteria
        for (auto [num, is_inverted]: controls_) {
            for (auto [num2, is_inverted2]: gate.controls_) {
                if (num == num2 && is_inverted && !is_inverted2) {
                    return true;
                }
                if (num == num2 && !is_inverted && is_inverted2) {
                    return true;
                }
            }
        }
        return false;
    }

    if ((type_ == GateType::SWAP || type_ == GateType::CSWAP) &&
        (gate.type_ == GateType::SWAP || gate.type_ == GateType::CSWAP)) {
        auto t11 = nests_.front();
        auto t12 = nests_.back();
        auto t21 = gate.nests_.front();
        auto t22 = gate.nests_.back();

        // first criteria is already checked

        // second criteria
        if (!(t11 == t21 || t11 == t22 || t12 == t21 || t12 == t22) &&
            std::find_if(gate.controls_.begin(), gate.controls_.end(),
                         [t11, t12](const auto p) {
                             return p.first == t11 || p.first == t12;
                         }) == gate.controls_.end() &&
            std::find_if(controls_.begin(), controls_.end(),
                         [t21, t22](const auto p) {
                             return p.first == t21 || p.first == t22;
                         }) == controls_.end()) {
            return true;
        }

        if (type_ == GateType::CSWAP && gate.type_ == GateType::CSWAP) {
            auto control1 = controls_.front();
            auto control2 = gate.controls_.front();
            if (control1.first == control2.first && control1.second != control2.second) {
                return true;
            }
        }

        return false;
    }

    auto g_swap = (type_ == GateType::SWAP || type_ == GateType::CSWAP) ? *this : gate;
    auto g_non_swap = (type_ != GateType::SWAP && type_ != GateType::CSWAP) ? *this : gate;

    auto t = g_non_swap.nests_.front();
    auto t1 = g_swap.nests_.front();
    auto t2 = g_swap.nests_.back();

    // first criteria
    // (t∉C∪T)⋀(T∩(I∪J)=∅)
    if ((t != t1 && t != t2) &&
        std::find_if(g_non_swap.controls_.begin(), g_non_swap.controls_.end(),
                     [t1, t2](const auto p) {
                         return p.first == t1 || p.first == t2;
                     }) == g_non_swap.controls_.end()) {
        if (g_swap.type_ == GateType::CSWAP) {
            return t != g_swap.controls_.front().first;
        }
        return true;
    }

    // second criteria
    // (t∉C)⋀((T⊆I)⋁(T⊆J))
    if (std::count_if(g_non_swap.controls_.begin(), g_non_swap.controls_.end(),
                      [t1, t2](const auto p) {
                          return p.second && (p.first == t1 || p.first == t2);
                      }) == 2 ||
        std::count_if(g_non_swap.controls_.begin(), g_non_swap.controls_.end(),
                      [t1, t2](const auto p) {
                          return !p.second && (p.first == t1 || p.first == t2);
                      }) == 2) {
        if (g_swap.type_ == GateType::CSWAP) {
            return t != g_swap.controls_.front().first;
        }
        return true;
    }

    if (g_swap.type_ == GateType::CSWAP) {
        // third criteria
        // (C∩I≠∅)⋁(C∩J≠∅)
        if (std::find_if(g_non_swap.controls_.begin(), g_non_swap.controls_.end(),
                         [&g_swap](const auto p) {
                             return p.first == g_swap.controls_.front().first &&
                                    p.second != g_swap.controls_.front().second;
                         }) != g_non_swap.controls_.end()) {
            return true;
        }
    }

    return false;
}

void Gate::act(binary_vector &vec) const {
    if (vec.size() != dim_) {
        throw GateException("Input vector must have length equals to the Gate dimension");
    }
    if (type_ == GateType::NOT) {
        vec[nests_.front()] = !vec[nests_.front()];
    } else if (type_ == GateType::CNOT) {
        vec[nests_.front()] = vec[nests_.front()] ^ vec[controls_.front().first] ^ !controls_.front().second;
    } else if (type_ == GateType::kCNOT) {
        bool control_signal = true;
        for (const auto &[num, is_direct]: controls_) {
            control_signal = control_signal && (vec[num] ^ !is_direct);
        }
        vec[nests_.front()] = vec[nests_.front()] ^ control_signal;
    } else if (type_ == GateType::SWAP) {
        swap(vec[nests_.front()], vec[nests_.back()]);
    } else if (type_ == GateType::CSWAP) {
        if ((vec[controls_.front().first] && controls_.front().second) ||
            (!vec[controls_.front().first] && !controls_.front().second)) {
            swap(vec[nests_.front()], vec[nests_.back()]);
        }
    }
}

void Gate::act(cf_set &vec) const {
    if (vec.size() != dim_) {
        throw GateException("Input coordinate boolean functions vector must have length equals to the Gate dimension");
    }
    if (!std::all_of(vec.begin(), vec.end(),
                     [bf_size = std::pow(2, dim_)](const auto &v) {
                         return v.size() == bf_size;
                     })) {
        throw GateException("Coordinate boolean functions must have the same dimensions as Gate");
    }

    if (type_ == GateType::NOT) {
        ~vec[nests_.front()];
    } else if (type_ == GateType::CNOT) {
        vec[nests_.front()] += vec[controls_.front().first] + BooleanFunction(!controls_.front().second, dim_);
    } else if (type_ == GateType::kCNOT) {
        BooleanFunction control_signal(true, dim_);
        for (const auto &[num, is_direct]: controls_) {
            if (!is_direct) {
                control_signal *= (vec[num] + BooleanFunction(true, dim_));
            } else {
                control_signal *= vec[num];
            }
        }
        vec[nests_.front()] += control_signal;
    } else if (type_ == GateType::SWAP) {
        vec[nests_.front()] += vec[nests_.back()];
        vec[nests_.back()] += vec[nests_.front()];
        vec[nests_.front()] += vec[nests_.back()];
    } else if (type_ == GateType::CSWAP) {
        BooleanFunction term = vec[controls_.front().first] * (vec[nests_.front()] + vec[nests_.back()]);
        if (!controls_.front().second) {
            vec[nests_.front()] += vec[nests_.back()];
            vec[nests_.back()] += vec[nests_.front()];
            vec[nests_.front()] += vec[nests_.back()];
        }
        vec[nests_.front()] += term;
        vec[nests_.back()] += term;
    }
}

bool Gate::operator==(const Gate &g) const {
    return (type_ == g.type_ && dim_ && g.dim_ && nests_ == g.nests_ && controls_ == g.controls_);
}

void Gate::validate_() const {
    for (auto num: nests_) {
        if (num > dim_ - 1) {
            throw GateException("Invalid nest line: " + std::to_string(num));
        }
        if (std::count(nests_.begin(), nests_.end(), num) != 1) {
            throw GateException("Nest line selected more that once: " + std::to_string(num));
        }
    }
    for (const auto &[num, is_direct]: controls_) {
        if (num > dim_ - 1) {
            throw GateException("Invalid control line: " + std::to_string(num));
        }
        if (std::find(nests_.begin(), nests_.end(), num) != nests_.end()) {
            throw GateException("Line selected as nest and control: " + std::to_string(num));
        }
        if (std::count_if(controls_.begin(), controls_.end(), [num](const auto p) { return p.first == num; }) != 1) {
            throw GateException("Nest line selected more that once: " + std::to_string(num));
        }
    }
    switch (type_) {
        case GateType::NOT:
            if (nests_.size() != 1) {
                throw GateException("Gate NOT should have an only nest line");
            }
            if (!controls_.empty()) {
                throw GateException("Gate NOT should not have control lines");
            }
            break;
        case GateType::CNOT:
            if (dim_ < 2) {
                throw GateException("Gate CNOT should have dimension equals at least 2");
            }
            if (nests_.size() != 1) {
                throw GateException("Gate CNOT should have an only nest line");
            }
            if (controls_.size() != 1) {
                throw GateException("Gate CNOT should have an only control line");
            }
            break;
        case GateType::kCNOT:
            if (dim_ < 2) {
                throw GateException("Gate kCNOT should have dimension equals at least 2");
            }
            if (nests_.size() != 1) {
                throw GateException("Gate kCNOT should have an only nest line");
            }
            if (controls_.empty()) {
                throw GateException("Gate kCNOT should have at least one control line");
            }
            break;
        case GateType::SWAP:
            if (dim_ < 2) {
                throw GateException("Gate SWAP should have dimension equals at least 2");
            }
            if (nests_.size() != 2) {
                throw GateException("Gate SWAP should have two nest lines");
            }
            if (!controls_.empty()) {
                throw GateException("Gate SWAP should not have control lines");
            }
            break;
        case GateType::CSWAP:
            if (dim_ < 3) {
                throw GateException("Gate CSWAP should have dimension equals at least 3");
            }
            if (nests_.size() != 2) {
                throw GateException("Gate CSWAP should have two nest lines");
            }
            if (controls_.size() != 1) {
                throw GateException("Gate CSWAP should have an only control line");
            }
            break;
        default:
            throw GateException("Unknown gate type: " + std::to_string(static_cast<int>(type_)));
    }
}

void Gate::init_(GateType type, const std::vector<size_t> &nests, const std::vector<control> &controls, size_t dim) {
    type_ = type;
    dim_ = dim;
    nests_ = nests;
    controls_ = controls;

    this->validate_();

    std::sort(nests_.begin(), nests_.end());
    std::sort(controls_.begin(), controls_.end());
}

void Gate::swap_lines_(const Gate &g) {
    if (g.type_ != GateType::SWAP) {
        throw GateException("Impossible to swap gate lines with not-SWAP gate");
    }
    auto t1 = g.nests_.front();
    auto t2 = g.nests_.back();

    for (auto &n: nests_) {
        if (n == t1) {
            n = t2;
        } else if (n == t2) {
            n = t1;
        }
    }

    for (auto &[c, is_inverted]: controls_) {
        if (c == t1) {
            c = t2;
        } else if (c == t2) {
            c = t1;
        }
    }

    this->validate_();
    std::sort(nests_.begin(), nests_.end());
    std::sort(controls_.begin(), controls_.end());
}

std::ostream &operator<<(std::ostream &out, const Gate &g) noexcept {
    std::string gate_name;
    switch (g.type_) {
        case GateType::NOT:
            gate_name = "NOT";
            break;
        case GateType::CNOT:
            gate_name = "CNOT";
            break;
        case GateType::kCNOT:
            gate_name = "kCNOT";
            break;
        case GateType::SWAP:
            gate_name = "SWAP";
            break;
        case GateType::CSWAP:
            gate_name = "CSWAP";
            break;
    }
    std::string gate_params;
    for (auto p: g.nests_) {
        gate_params += std::to_string(p) + ", ";
    }
    gate_params.erase(gate_params.size() - 2);  // remove ", " from the end
    if (!g.controls_.empty()) {
        gate_params += "; ";
        for (const auto &[num, is_direct]: g.controls_) {
            if (!is_direct) {
                gate_params += '!';
            }
            gate_params += std::to_string(num) + ", ";
        }
        gate_params.erase(gate_params.size() - 2);  // remove ", " from the end
    }
    return out << gate_name << '(' << gate_params << ')';
}

// Circuit

Circuit::Circuit(size_t lines_num, size_t memory_lines_num) {
    if (!lines_num) {
        throw CircuitException("Circuit must have at least one line");
    }
    if (memory_lines_num >= lines_num) {
        throw CircuitException("Circuit must have a number of memory lines not less than the number of lines");
    }
    dim_ = lines_num;
    memory_ = memory_lines_num;
}

Circuit::Circuit(const std::vector<Gate> &vec, size_t memory_lines_num) {
    if (vec.empty()) {
        throw CircuitException("Empty gates vector");
    }
    memory_ = memory_lines_num;
    dim_ = vec.front().dim();

    if (memory_lines_num >= dim_) {
        throw CircuitException("Circuit must have a number of memory lines not less than the number of lines");
    }

    for (const auto &g: vec) {
        this->add(g);
    }
}

Circuit::Circuit(const std::string &s) {
    by_string_(s);
}

Circuit::Circuit(std::istream &s) {
    std::stringstream ss;
    ss << s.rdbuf();
    by_string_(ss.str());
}

size_t Circuit::dim() const noexcept {
    return dim_;
}

size_t Circuit::memory() const noexcept {
    return memory_;
}

void Circuit::set_memory(size_t memory_lines_num) {
    if (memory_lines_num >= dim_) {
        throw CircuitException("Circuit must have a number of memory lines not less than the number of lines");
    }
    memory_ = memory_lines_num;
}

void Circuit::act(binary_vector &vec) const {
    if (vec.size() != dim_) {
        throw CircuitException("Input vector must have length equals to the Circuit dimension");
    }
    if (memory_) {
        for (size_t i = vec.size() - 1; i > dim_ - memory_ - 1; i--) {
            vec[i] = false;
        }
    }
    for (const auto &g: gates_) {
        g.act(vec);
    }
}

void Circuit::act(cf_set &vec) const {
    if (vec.size() != dim_) {
        throw CircuitException(
                "Input coordinate boolean functions vector must have length equals to the Circuit dimension");
    }
    if (!std::all_of(vec.begin(), vec.end(),
                     [bf_size = std::pow(2, dim_)](const auto &v) {
                         return v.size() == bf_size;
                     })) {
        throw CircuitException("Coordinate boolean functions must have the same dimensions as Circuit");
    }
    for (const auto &g: gates_) {
        g.act(vec);
    }
    if (!memory_) {
        return;
    }
    for (auto &bf: vec) {
        binary_vector bf_values;
        for (size_t i = 0; i < bf.size(); i++) {
            if (!(i % (1 << memory_))) {
                bf_values.push_back(bf.vector()[i]);
            }
        }
        bf = BooleanFunction(bf_values);
    }
}

void Circuit::add(const Gate &g) {
    if (g.dim() != dim_) {
        throw CircuitException("Circuit and Gate must have equal dimensions");
    }
    gates_.push_back(g);
}

void Circuit::insert(const Gate &g, size_t pos) {
    if (g.dim() != dim_) {
        throw CircuitException("Circuit and Gate must have equal dimensions");
    }
    if (pos > gates_.size()) {
        throw CircuitException("Provided pos out of range");
    }
    gates_.insert(gates_.begin() + pos, g);
}

BinaryMapping Circuit::produce_mapping() const noexcept {
    cf_set vec_bf;
    vec_bf.reserve(this->dim());
    for (size_t i = 0; i < this->dim(); i++) {
        vec_bf.emplace_back(i, this->dim());
    }
    this->act(vec_bf);
    return BinaryMapping(vec_bf);
}

//void Circuit::reduce() noexcept {
//}

bool Circuit::operator==(const Circuit &c) const {
    return this->produce_mapping() == c.produce_mapping();
}

bool Circuit::schematically_equal(const Circuit &c) const noexcept {
    return dim_ == c.dim_ && memory_ == c.memory_ && gates_ == c.gates_;
}

size_t Circuit::move_swap_left() {
    bool is_any_swaps = false;
    bool is_only_swaps = true;
    bool is_swap_in_left = true;
    size_t swap_count = 0;

    for (const auto &gate: gates_) {
        if (gate.type() == GateType::SWAP) {
            swap_count++;
            is_any_swaps = true;
            if (!is_only_swaps) {
                is_swap_in_left = false;
            }
        } else {
            is_only_swaps = false;
        }
    }

    if (!is_any_swaps || is_only_swaps || is_swap_in_left) {
        return swap_count;
    }

    std::vector<Gate> non_swap_gates;
    std::vector<Gate> swap_gates;
    non_swap_gates.reserve(gates_.size());

    size_t i = gates_.size();
    do {
        i--;
        if (gates_[i].type() == GateType::SWAP) {
            if (i) {
                size_t j = i;
                do {
                    j--;
                    gates_[j].swap_lines_(gates_[i]);
                } while (j);
            }
            swap_gates.push_back(gates_[i]);
            continue;
        }
        non_swap_gates.insert(non_swap_gates.begin(), gates_[i]);
    } while (i);

    gates_.clear();
    gates_ = swap_gates;
    gates_.insert(gates_.end(), non_swap_gates.begin(), non_swap_gates.end());
    return swap_count;
}

std::ostream &operator<<(std::ostream &out, const Circuit &c) noexcept {
    out << "Lines: " << c.dim_;
    if (c.memory_) {
        out << "; " << c.memory_;
    }
    out << '\n';
    for (const auto &g: c.gates_) {
        out << g << '\n';
    }
    return out;
}

void Circuit::by_string_(const std::string &s) {
    if (s.empty()) {
        throw CircuitException("Empty string");
    }
    std::stringstream ss(s);
    std::string line;

    // lines number
    while (getline(ss, line, '\n')) {
        trim(line);
        if (line.empty() || line.front() == '#') {
            line.clear();
            continue;
        }
        to_lower(line);
        auto lines_word_pos = line.find("lines:");
        if (lines_word_pos) {
            throw CircuitException("Invalid string");
        }
        line = line.substr(lines_word_pos + 6);
        int dim = 0;
        int memory = 0;
        try {
            auto nums = string_to_num_vector<int>(line, ';');
            if (nums.empty() || nums.size() > 2) {
                throw CircuitException("Invalid lines number: " + line);
            }
            dim = nums.front();
            if (nums.size() == 2) {
                memory = nums.back();
            }
        } catch (...) {
            throw CircuitException("Invalid lines number: " + line);
        }
        dim_ = dim;
        if (memory < 0) {
            throw CircuitException("Number of memory lines must be a positive integer");
        }
        if (memory >= dim) {
            throw CircuitException("Circuit must have a number of memory lines not less than the number of lines");
        }
        memory_ = memory;
        break;
    }
    if (line.empty()) {
        throw CircuitException("Invalid string");
    }

    // gate
    while (getline(ss, line, '\n')) {
        trim(line);
        if (line.empty() || line.front() == '#') {
            continue;
        }
        gates_.emplace_back(line, dim_);
    }
}
