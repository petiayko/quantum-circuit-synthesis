#include "gates.hpp"

Gate::Gate(int type, const std::unordered_set<size_t> &nests, const std::unordered_set<size_t> &controls, size_t dim) {
    init_(type, nests, controls, dim);
}

Gate::Gate(const std::string &s, size_t dim) {
    // NAME(0, 1; 2, 3)
    if (s.empty()) {
        throw std::runtime_error{"Empty string"};
    }
    std::string s_copy(s);
    std::transform(s_copy.begin(), s_copy.end(), s_copy.begin(), [](char ch) {
        return std::tolower(ch);
    });
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

    int type = -1;
//    std::unordered_set<size_t> nests;
//    std::unordered_set<size_t> controls;
    if (gate_name == "not") {
        type = NOT;
        if (semicolumn_pos != std::string::npos) {
            throw std::runtime_error{"Invalid NOT gate format"};
        }
    } else if (gate_name == "cnot") {
        type = CNOT;
        if (semicolumn_pos == std::string::npos) {
            throw std::runtime_error{"Invalid CNOT gate format"};
        }
    } else if (gate_name == "kcnot") {
        type = kCNOT;
        if (semicolumn_pos == std::string::npos) {
            throw std::runtime_error{"Invalid kCNOT gate format"};
        }
    } else if (gate_name == "swap") {
        type = SWAP;
        if (semicolumn_pos != std::string::npos) {
            throw std::runtime_error{"Invalid SWAP gate format"};
        }
    } else if (gate_name == "cswap") {
        type = CSWAP;
        if (semicolumn_pos == std::string::npos) {
            throw std::runtime_error{"Invalid CSWAP gate format"};
        }
    } else {
        throw std::runtime_error{"Unknown gate type: " + gate_name};
    }

    auto nests = string_to_num_vector(nests_line, ',');
    auto controls = string_to_num_vector(controls_line, ',');
    init_(type, {nests.begin(), nests.end()}, {controls.begin(), controls.end()}, dim);
}

size_t Gate::dim() const noexcept {
    return dim_;
}


//void Gate::act(std::vector<BooleanFunction> &vec) const {
//
//}

bool Gate::operator==(const Gate &g) {
    return (type_ == g.type_ && dim_ && g.dim_ && nests_ == g.nests_ && controls_ == g.controls_);
}

void Gate::init_(int type, const std::unordered_set<size_t> &nests,
                 const std::unordered_set<size_t> &controls, size_t dim) {
    for (auto num: nests) {
        if (num > dim - 1) {
            throw std::runtime_error{std::string("Invalid nest line: ") + std::to_string(num)};
        }
    }
    for (auto num: controls) {
        if (num > dim - 1) {
            throw std::runtime_error{std::string("Invalid control line: ") + std::to_string(num)};
        }
        if (nests.find(num) != nests.end()) {
            throw std::runtime_error{std::string("Line selected as nest and control: ") + std::to_string(num)};
        }
    }
    switch (type) {
        case NOT:
            if (nests.size() != 1) {
                throw std::runtime_error{"Gate NOT should have an only nest line"};
            }
            if (!controls.empty()) {
                throw std::runtime_error{"Gate NOT should not have control lines"};
            }
            break;
        case CNOT:
            if (nests.size() != 1) {
                throw std::runtime_error{"Gate CNOT should have an only nest line"};
            }
            if (controls.empty() || controls.size() != 1) {
                throw std::runtime_error{"Gate CNOT should have an only nest line"};
            }
            break;
        case kCNOT:
            if (nests.size() != 1) {
                throw std::runtime_error{"Gate kCNOT should have an only nest line"};
            }
            if (controls.empty()) {
                throw std::runtime_error{"Gate kCNOT should have at least one nest line"};
            }
            break;
        case SWAP:
            if (nests.size() != 2) {
                throw std::runtime_error{"Gate SWAP should have two nest line"};
            }
            if (!controls.empty()) {
                throw std::runtime_error{"Gate SWAP should not have control lines"};
            }
            break;
        case CSWAP:
            if (nests.size() != 2) {
                throw std::runtime_error{"Gate CSWAP should have two nest line"};
            }
            if (controls.empty() || controls.size() != 1) {
                throw std::runtime_error{"Gate CSWAP should have at least one nest line"};
            }
            break;
        default:
            throw std::runtime_error{std::string("Unknown gate type: ") + std::to_string(type)};
    }

    type_ = type;
    dim_ = dim;
    nests_ = nests;
    controls_ = controls;
}

std::ostream &operator<<(std::ostream &out, const Gate &g) noexcept {
    std::string gate_name;
    switch (g.type_) {
        case NOT:
            gate_name = "NOT";
            break;
        case CNOT:
            gate_name = "CNOT";
            break;
        case kCNOT:
            gate_name = "kCNOT";
            break;
        case SWAP:
            gate_name = "SWAP";
            break;
        case CSWAP:
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

size_t Circuit::dim() const noexcept {
    return dim_;
}

void Circuit::add(const Gate &g) {
    if (g.dim() != dim_) {
        throw std::runtime_error{"Circuit and gate should have equal dimensions"};
    }
    gates_.push_back(g);
}

std::ostream &operator<<(std::ostream &out, const Circuit &c) noexcept {
    out << "Lines: " << c.dim_ << '\n';
    for (const auto &g: c.gates_) {
        out << g << '\n';
    }
    return out;
}
