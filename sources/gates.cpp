#include "gates.hpp"

Gate::Gate(GateType type, const std::vector<size_t> &nests, const std::vector<size_t> &controls, size_t dim) {
    init_(type, nests, controls, dim);
}

Gate::Gate(const std::string &s, size_t dim) {
    // NAME(0, 1; 2, 3)
    if (s.empty()) {
        throw std::runtime_error{"Empty string"};
    }
    std::string s_copy(s);
    to_lower(s_copy);
    trim(s_copy);

    auto open_bracket_pos = s_copy.find('(');
    if (open_bracket_pos == std::string::npos) {
        throw std::runtime_error{"Invalid string"};
    }
    auto gate_name = s_copy.substr(0, open_bracket_pos);
    trim(gate_name);
    s_copy = s_copy.substr(open_bracket_pos + 1);

    auto close_bracket_pos = s_copy.find(')');
    if (s_copy.empty() || open_bracket_pos == std::string::npos) {
        throw std::runtime_error{"Invalid string"};
    }
    auto gate_params = s_copy.substr(0, close_bracket_pos);
    trim(gate_params);
    s_copy = s_copy.substr(close_bracket_pos + 1);

    if (!s_copy.empty()) {
        throw std::runtime_error{"Invalid string"};
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
            throw std::runtime_error{"Invalid NOT gate format"};
        }
    } else if (gate_name == "cnot") {
        type = GateType::CNOT;
        if (semicolumn_pos == std::string::npos) {
            throw std::runtime_error{"Invalid CNOT gate format"};
        }
    } else if (gate_name == "kcnot") {
        type = GateType::kCNOT;
        if (semicolumn_pos == std::string::npos) {
            throw std::runtime_error{"Invalid kCNOT gate format"};
        }
    } else if (gate_name == "swap") {
        type = GateType::SWAP;
        if (semicolumn_pos != std::string::npos) {
            throw std::runtime_error{"Invalid SWAP gate format"};
        }
    } else if (gate_name == "cswap") {
        type = GateType::CSWAP;
        if (semicolumn_pos == std::string::npos) {
            throw std::runtime_error{"Invalid CSWAP gate format"};
        }
    } else {
        throw std::runtime_error{"Unknown gate type: " + gate_name};
    }

    init_(type, string_to_num_vector(nests_line, ','), string_to_num_vector(controls_line, ','), dim);
}

size_t Gate::dim() const noexcept {
    return dim_;
}

void Gate::act(std::vector<bool> &vec) const {
    if (vec.size() != dim_) {
        throw std::runtime_error{"Vector should have length equals to the Gate dimension"};
    }
    if (type_ == GateType::NOT) {
        vec[nests_.front()] = !vec[nests_.front()];
    } else if (type_ == GateType::CNOT) {
        vec[nests_.front()] = vec[nests_.front()] != vec[controls_.front()];
    } else if (type_ == GateType::kCNOT) {
        bool control_signal = true;
        for (auto num: controls_) {
            control_signal = control_signal && vec[num];
        }
        vec[nests_.front()] = vec[nests_.front()] != control_signal;
    } else if (type_ == GateType::SWAP) {
        swap(vec[nests_.front()], vec[nests_.back()]);
    } else if (type_ == GateType::CSWAP) {
        if (vec[controls_.front()]) {
            swap(vec[nests_.front()], vec[nests_.back()]);
        }
    }
}

void Gate::act(std::vector<BooleanFunction> &vec) const {
    if (vec.size() != dim_) {
        throw std::runtime_error{"Vector should have length equals to the Gate dimension"};
    }
    if (!std::all_of(vec.begin(), vec.end(),
                     [bf_size = std::pow(2, dim_)](const auto &v) {
                         return v.size() == bf_size;
                     })) {
        throw std::runtime_error{"All boolean functions should have the same dimensions as Gate"};
    }

    if (type_ == GateType::NOT) {
        vec[nests_.front()] = ~vec[nests_.front()];
    } else if (type_ == GateType::CNOT) {
        vec[nests_.front()] += vec[controls_.front()];
    } else if (type_ == GateType::kCNOT) {
        BooleanFunction control_signal(true, dim_);
        for (auto num: controls_) {
            control_signal *= vec[num];
        }
        vec[nests_.front()] += control_signal;
    } else if (type_ == GateType::SWAP) {
        BooleanFunction temp = vec[nests_.front()];
        vec[nests_.front()] = vec[nests_.back()];
        vec[nests_.back()] = temp;
    } else if (type_ == GateType::CSWAP) {
        BooleanFunction front_bf = (vec[controls_.front()] + BooleanFunction(true, dim_)) * vec[nests_.front()] |
                                   vec[controls_.front()] * vec[nests_.back()];
        BooleanFunction back_bf = vec[controls_.front()] * vec[nests_.front()] |
                                  (vec[controls_.front()] + BooleanFunction(true, dim_)) * vec[nests_.back()];
        vec[nests_.front()] = front_bf;
        vec[nests_.back()] = back_bf;
    }
}

bool Gate::operator==(const Gate &g) const {
    return (type_ == g.type_ && dim_ && g.dim_ && nests_ == g.nests_ && controls_ == g.controls_);
}

void Gate::init_(GateType type, const std::vector<size_t> &nests, const std::vector<size_t> &controls, size_t dim) {
    for (auto num: nests) {
        if (num > dim - 1) {
            throw std::runtime_error{std::string("Invalid nest line: ") + std::to_string(num)};
        }
        if (std::count(nests.begin(), nests.end(), num) != 1) {
            throw std::runtime_error{std::string("Nest line selected more the once: ") + std::to_string(num)};
        }
    }
    for (auto num: controls) {
        if (num > dim - 1) {
            throw std::runtime_error{std::string("Invalid control line: ") + std::to_string(num)};
        }
        if (std::count(controls.begin(), controls.end(), num) != 1) {
            throw std::runtime_error{std::string("Nest line selected more the once: ") + std::to_string(num)};
        }
        if (std::find(nests.begin(), nests.end(), num) != nests.end()) {
            throw std::runtime_error{std::string("Line selected as nest and control: ") + std::to_string(num)};
        }
    }
    switch (type) {
        case GateType::NOT:
            if (nests.size() != 1) {
                throw std::runtime_error{"Gate NOT should have an only nest line"};
            }
            if (!controls.empty()) {
                throw std::runtime_error{"Gate NOT should not have control lines"};
            }
            break;
        case GateType::CNOT:
            if (dim < 2) {
                throw std::runtime_error{"Gate CNOT should have dimension equals at least 2"};
            }
            if (nests.size() != 1) {
                throw std::runtime_error{"Gate CNOT should have an only nest line"};
            }
            if (controls.empty() || controls.size() != 1) {
                throw std::runtime_error{"Gate CNOT should have an only nest line"};
            }
            break;
        case GateType::kCNOT:
            if (dim < 2) {
                throw std::runtime_error{"Gate kCNOT should have dimension equals at least 2"};
            }
            if (nests.size() != 1) {
                throw std::runtime_error{"Gate kCNOT should have an only nest line"};
            }
            if (controls.empty()) {
                throw std::runtime_error{"Gate kCNOT should have at least one nest line"};
            }
            break;
        case GateType::SWAP:
            if (dim < 2) {
                throw std::runtime_error{"Gate SWAP should have dimension equals at least 2"};
            }
            if (nests.size() != 2) {
                throw std::runtime_error{"Gate SWAP should have two nest line"};
            }
            if (!controls.empty()) {
                throw std::runtime_error{"Gate SWAP should not have control lines"};
            }
            break;
        case GateType::CSWAP:
            if (dim < 3) {
                throw std::runtime_error{"Gate CSWAP should have dimension equals at least 3"};
            }
            if (nests.size() != 2) {
                throw std::runtime_error{"Gate CSWAP should have two nest line"};
            }
            if (controls.empty() || controls.size() != 1) {
                throw std::runtime_error{"Gate CSWAP should have at least one nest line"};
            }
            break;
        default:
            throw std::runtime_error{std::string("Unknown gate type: ") + std::to_string(static_cast<int>(type))};
    }

    type_ = type;
    dim_ = dim;
    nests_ = nests;
    controls_ = controls;
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
    gate_params = gate_params.substr(0, gate_params.size() - 2);
    if (!g.controls_.empty()) {
        gate_params += "; ";
        for (auto p: g.controls_) {
            gate_params += std::to_string(p) + ", ";
        }
        gate_params = gate_params.substr(0, gate_params.size() - 2);
    }
    return out << gate_name << '(' << gate_params << ')';
}

// Circuit

Circuit::Circuit(size_t lines_num) {
    dim_ = lines_num;
}

Circuit::Circuit(const std::vector<Gate> &vec) {
    if (vec.empty()) {
        return;
    }
    dim_ = vec.front().dim();
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

void Circuit::act(std::vector<bool> &vec) const {
    if (vec.size() != dim_) {
        throw std::runtime_error{"Vector length should be equal to Circuit dimension"};
    }
    for (const auto &g: gates_) {
        g.act(vec);
    }
}

void Circuit::act(std::vector<BooleanFunction> &vec) const {
    if (vec.size() != dim_) {
        throw std::runtime_error{"Vector should have length equals to the Circuit dimension"};
    }
    if (!std::all_of(vec.begin(), vec.end(),
                     [bf_size = std::pow(2, dim_)](const auto &v) {
                         return v.size() == bf_size;
                     })) {
        throw std::runtime_error{"All boolean functions should have the same dimensions as Circuit"};
    }
    for (const auto &g: gates_) {
        g.act(vec);
    }
}

void Circuit::add(const Gate &g) {
    if (g.dim() != dim_) {
        throw std::runtime_error{"Circuit and gate should have equal dimensions"};
    }
    gates_.push_back(g);
}

bool Circuit::operator==(const Circuit &c) const {
    return (dim_ == c.dim_ && gates_ == c.gates_);
}

std::ostream &operator<<(std::ostream &out, const Circuit &c) noexcept {
    out << "Lines: " << c.dim_ << '\n';
    for (const auto &g: c.gates_) {
        out << g << '\n';
    }
    return out;
}

void Circuit::by_string_(const std::string &s) {
    if (s.empty()) {
        throw std::runtime_error{"Empty string"};
    }
    std::stringstream ss(s);
    std::string line;

    while (getline(ss, line, '\n')) {
        trim(line);
        if (line.empty() || line.front() == '#') {
            line.clear();
            continue;
        }
        to_lower(line);
        auto lines_word_pos = line.find("lines:");
        if (lines_word_pos == std::string::npos) {
            throw std::runtime_error{"Invalid string"};
        }
        line = line.substr(lines_word_pos + 6);
        int dim = 0;
        if (!try_string_to_decimal(line, dim)) {
            throw std::runtime_error{std::string("Invalid lines number: ") + line};
        }
        if (dim < 1) {
            throw std::runtime_error{"Invalid number of lines"};
        }
        dim_ = dim;
        break;
    }
    if (line.empty()) {
        throw std::runtime_error{"Invalid string"};
    }

    while (getline(ss, line, '\n')) {
        if (line.empty() || line.front() == '#') {
            continue;
        }
        gates_.emplace_back(line, dim_);
    }
}
