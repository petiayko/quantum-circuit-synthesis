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

void Gate::init_(GateType type, const std::vector<size_t> &nests, const std::vector<control> &controls, size_t dim) {
    for (auto num: nests) {
        if (num > dim - 1) {
            throw GateException("Invalid nest line: " + std::to_string(num));
        }
        if (std::count(nests.begin(), nests.end(), num) != 1) {
            throw GateException("Nest line selected more that once: " + std::to_string(num));
        }
    }
    for (const auto &[num, is_direct]: controls) {
        if (num > dim - 1) {
            throw GateException("Invalid control line: " + std::to_string(num));
        }
        if (std::find(nests.begin(), nests.end(), num) != nests.end()) {
            throw GateException("Line selected as nest and control: " + std::to_string(num));
        }
        if (std::count_if(controls.begin(), controls.end(), [num](const auto p) { return p.first == num; }) != 1) {
            throw GateException("Nest line selected more that once: " + std::to_string(num));
        }
    }
    switch (type) {
        case GateType::NOT:
            if (nests.size() != 1) {
                throw GateException("Gate NOT should have an only nest line");
            }
            if (!controls.empty()) {
                throw GateException("Gate NOT should not have control lines");
            }
            break;
        case GateType::CNOT:
            if (dim < 2) {
                throw GateException("Gate CNOT should have dimension equals at least 2");
            }
            if (nests.size() != 1) {
                throw GateException("Gate CNOT should have an only nest line");
            }
            if (controls.size() != 1) {
                throw GateException("Gate CNOT should have an only control line");
            }
            break;
        case GateType::kCNOT:
            if (dim < 2) {
                throw GateException("Gate kCNOT should have dimension equals at least 2");
            }
            if (nests.size() != 1) {
                throw GateException("Gate kCNOT should have an only nest line");
            }
            if (controls.empty()) {
                throw GateException("Gate kCNOT should have at least one control line");
            }
            break;
        case GateType::SWAP:
            if (dim < 2) {
                throw GateException("Gate SWAP should have dimension equals at least 2");
            }
            if (nests.size() != 2) {
                throw GateException("Gate SWAP should have two nest lines");
            }
            if (!controls.empty()) {
                throw GateException("Gate SWAP should not have control lines");
            }
            break;
        case GateType::CSWAP:
            if (dim < 3) {
                throw GateException("Gate CSWAP should have dimension equals at least 3");
            }
            if (nests.size() != 2) {
                throw GateException("Gate CSWAP should have two nest lines");
            }
            if (controls.size() != 1) {
                throw GateException("Gate CSWAP should have an only control line");
            }
            break;
        default:
            throw GateException("Unknown gate type: " + std::to_string(static_cast<int>(type)));
    }

    type_ = type;
    dim_ = dim;
    nests_ = nests;
    controls_ = controls;

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
    if (memory_lines_num && memory_lines_num >= lines_num) {
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

bool Circuit::simplify() noexcept {
    return false;
}

bool Circuit::operator==(const Circuit &c) const {
    return (dim_ == c.dim_ && memory_ == c.memory_ && gates_ == c.gates_);
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
