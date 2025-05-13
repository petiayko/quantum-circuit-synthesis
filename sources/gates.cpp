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

    int type = -1;
    std::unordered_set<size_t> nests;
    std::unordered_set<size_t> controls;
    if (gate_name == "not") {
        type = NOT;
        if (semicolumn_pos != std::string::npos) {
            throw std::runtime_error{"Invalid NOT gate format"};
        }
        size_t nest_line;
        if (!try_string_to_decimal(gate_params, nest_line)) {
            throw std::runtime_error{"Invalid NOT gate format"};
        }
    } else if (gate_name == "cnot") {
        type = CNOT;
        if (semicolumn_pos == std::string::npos) {
            throw std::runtime_error{"Invalid CNOT gate format"};
        }
        auto nests_s = gate_params.substr(0, semicolumn_pos);
        gate_params = gate_params.substr(semicolumn_pos + 1);
        auto controls_s = gate_params;
    } else if (gate_name == "kcnot") {
        type = kCNOT;
    } else if (gate_name == "swap") {
        type = SWAP;
    } else if (gate_name == "cswap") {
        type = CSWAP;
    } else {
        throw std::runtime_error{"Unknown gate type: " + gate_name};
    }

    init_(type, nests, controls, dim);
}

void Gate::init_(int type, const std::unordered_set<size_t> &nests, const std::unordered_set<size_t> &controls, size_t dim) {
    for (auto num: nests) {
        if (num > dim - 1) {
            throw std::runtime_error{std::string("Invalid nest line: ") + std::to_string(num)};
        }
    }
    for (auto num: controls) {
        if (num > dim - 1) {
            throw std::runtime_error{std::string("Invalid control line: ") + std::to_string(num)};
        }
        if (std::find(nests.begin(), nests.end(), num) != nests.end()) {
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
