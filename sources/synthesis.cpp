#include "synthesis.hpp"


Circuit synthesize(const BinaryMapping &bm, Algo algo, bool reduction) {
    if (algo == Algo::DUMMY) {
        return dummy_algorithm(bm, reduction);
    }
    if (algo == Algo::RW) {
        return RW_algorithm(bm, reduction);
    }
    if (algo == Algo::SS) {
        return SS_algorithm(bm, reduction);
    }
    throw SynthException("Unknown synthesis algorithm");
}

Circuit synthesize(const Substitution &sub, Algo algo, bool reduction) {
    if (algo == Algo::DUMMY) {
        return dummy_algorithm(sub, reduction);
    }
    if (algo == Algo::RW) {
        return RW_algorithm(sub, reduction);
    }
    if (algo == Algo::SS) {
        return SS_algorithm(sub, reduction);
    }
    throw SynthException("Unknown synthesis algorithm");
}

Circuit dummy_algorithm(const BinaryMapping &bm, bool reduction) {
    const auto bm_bf = bm.coordinate_functions();
    std::vector<std::vector<bool>> bm_anf;
    bm_anf.resize(bm_bf.size());
    std::transform(bm_bf.cbegin(), bm_bf.cend(), bm_anf.begin(), [](const BooleanFunction &bf) {
        return bf.mobius_transformation();
    });

    auto c_dim = bm.inputs_number() + bm.outputs_number();
    auto c = Circuit(c_dim);
    c.set_memory(bm.outputs_number());
    for (size_t i = 0; i < bm.outputs_number(); i++) {
        if (bm_anf[i].front()) {
            c.add(Gate(GateType::NOT, {bm.inputs_number() + i}, {}, c_dim));
        }
        for (size_t c_set = 1; c_set < bm_anf[i].size(); c_set++) {
            if (bm_anf[i][c_set]) {
                controls_type controls;
                for (const auto num: bits_mask(c_set, bm.inputs_number())) {
                    controls[num] = true;
                }
                c.add(Gate(GateType::kCNOT, {bm.inputs_number() + i}, controls, c_dim));
            }
        }
    }

    if (reduction) {
        c.reduce();
    }
    return c;
}

Circuit dummy_algorithm(const Substitution &sub, bool reduction) {
    if (!is_power_of_2(sub.power())) {
        throw SynthException("Substitution size must be power of 2");
    }
    return dummy_algorithm(BinaryMapping(sub), reduction);
}

void generate_controls_(size_t len, size_t max, size_t exclude, size_t start, std::vector<size_t> &current,
                        std::vector<std::vector<size_t>> &result) {
    if (current.size() == len) {
        result.push_back(current);
        return;
    }

    for (size_t i = start; i < max; i++) {
        if (i == exclude) {
            continue;
        }
        current.push_back(i);
        generate_controls_(len, max, exclude, i + 1, current, result);
        current.pop_back();
    }
}

std::vector<std::vector<size_t>> generate_controls(size_t len, size_t max, size_t exclude) {
    if (len > max - (exclude < max)) {
        return {};
    }

    std::vector<std::vector<size_t>> result;
    std::vector<size_t> current;
    generate_controls_(len, max, exclude, 0, current, result);
    return result;
}

std::vector<Gate> generate_gates(GateType type, size_t nest, const std::vector<size_t> &controls,
                                 size_t dim) {
    std::vector<Gate> result;
    for (int i = 0; i < 1 << controls.size(); i++) {
        controls_type marked_controls;
        auto mask = decimal_to_binary_v<int>(i, controls.size());
        for (size_t j = 0; j < mask.size(); j++) {
            marked_controls[controls[j]] = !mask[j];
        }
        result.emplace_back(type, std::vector<size_t>{nest}, marked_controls, dim);
    }

    return result;
}

Circuit RW_algorithm(const BinaryMapping &bm, bool reduction) {
    auto bm_extend = bm.extend();
    auto bm_cf = bm_extend.coordinate_functions();
    const auto outputs = bm_extend.outputs_number();

    auto c = Circuit(outputs);

    for (int i = outputs - 1;; i--) {
        if (c.produce_mapping() == bm_extend) {
            c.set_memory(bm_extend.inputs_number() - bm.inputs_number());
            return c;
        }

        if (i < 0) {
            i = outputs - 1;
        }
        size_t nest = i;

        if (bm_cf[nest] == BooleanFunction(nest, outputs)) {
            continue;
        } else if (bm_cf[nest] == ~BooleanFunction(nest, outputs)) {
            auto g = Gate(GateType::NOT, {nest}, {}, outputs);
            c.insert(g, 0);
            g.act(bm_cf);
            continue;
        }

        auto complexity = bm_cf[nest].complexity();
        auto max_complexity_diff = 0;
        Gate best_gate;

        // CNOT
        for (const auto &controls: generate_controls(1, outputs, i)) {
            for (const auto &gate: generate_gates(GateType::CNOT, nest, controls, outputs)) {
                gate.act(bm_cf);
                auto complexity_new = bm_cf[nest].complexity();
                if (complexity_new - complexity > max_complexity_diff) {
                    max_complexity_diff = complexity_new - complexity;
                    best_gate = gate;
                }
                gate.act(bm_cf);
            }
        }
        if (max_complexity_diff) {
            c.insert(best_gate, 0);
            best_gate.act(bm_cf);
            continue;
        }

        // kCNOT 2
        for (const auto &controls: generate_controls(2, outputs, i)) {
            for (const auto &gate: generate_gates(GateType::kCNOT, nest, controls, outputs)) {
                gate.act(bm_cf);
                auto complexity_new = bm_cf[nest].complexity();
                if (complexity_new - complexity > max_complexity_diff) {
                    max_complexity_diff = complexity_new - complexity;
                    best_gate = gate;
                }
                gate.act(bm_cf);
            }
        }
        if (max_complexity_diff) {
            c.insert(best_gate, 0);
            best_gate.act(bm_cf);
            continue;
        }

        // kCNOT 3
        for (const auto &controls: generate_controls(3, outputs, i)) {
            for (const auto &gate: generate_gates(GateType::kCNOT, nest, controls, outputs)) {
                gate.act(bm_cf);
                auto complexity_new = bm_cf[nest].complexity();
                if (complexity_new - complexity > max_complexity_diff) {
                    max_complexity_diff = complexity_new - complexity;
                    best_gate = gate;
                }
                gate.act(bm_cf);
            }
        }
        if (max_complexity_diff) {
            c.insert(best_gate, 0);
            best_gate.act(bm_cf);
            continue;
        }

        break;
    }

    for (size_t i = 0; i < outputs; i++) {
        if (bm_cf[i].vector().front()) {
            ~bm_cf[i];
            c.insert(Gate(GateType::NOT, {i}, {}, outputs), 0);
        }
    }

    try {
        for (size_t i = 0; i < outputs; i++) {
            for (size_t j = i + 1; j < outputs; j++) {
                if (bm_cf[i].variable() > bm_cf[j].variable()) {
                    std::swap(bm_cf[i], bm_cf[j]);
                    c.insert(Gate(GateType::SWAP, {i, j}, {}, outputs), 0);
                }
            }
        }
    } catch (const BFException &e) {
        throw SynthException("Unable to synthesize Circuit");
    }

    if (reduction) {
        c.reduce();
    }

    if (c.produce_mapping() != bm_extend) {
        LOG_DEBUG("The synthesized circuit produces an incorrect mapping", static_cast<std::string>(c));
        throw SynthException("Unable to synthesize Circuit");
    }

    c.set_memory(bm_extend.inputs_number() - bm.inputs_number());
    return c;
}

Circuit RW_algorithm(const Substitution &sub, bool reduction) {
    if (!is_power_of_2(sub.power())) {
        throw SynthException("Substitution size should be power of 2");
    }
    if (sub.is_identical()) {
        return Circuit(std::log2(sub.power()));
    }
    return RW_algorithm(BinaryMapping(sub), reduction);
}

Circuit SS_algorithm(const BinaryMapping &bm, bool reduction) {
    return SS_algorithm(Substitution(bm), reduction);
}

std::unordered_map<Gate, Substitution> generate_gates_substitutions(const size_t dim) {
    std::unordered_map<Gate, Substitution> result;
    Gate gate;

    for (size_t i = 0; i < dim; i++) {
        gate = Gate(GateType::NOT, {i}, {}, dim);
        result.insert({gate, gate.act()});
        for (size_t j = 0; j < dim; j++) {
            if (j == i) {
                continue;
            }
            gate = Gate(GateType::SWAP, {i, j}, {}, dim);
            result.insert({gate, gate.act()});
            for (bool control: {true, false}) {
                gate = Gate(GateType::CNOT, {i}, {{j, control}}, dim);
                result.insert({gate, gate.act()});
            }
            for (size_t k = 0; k < dim; k++) {
                if (k == i || k == j) {
                    continue;
                }
                for (bool control: {true, false}) {
                    gate = Gate(GateType::CSWAP, {i, j}, {{k, control}}, dim);
                    result.insert({gate, gate.act()});
                }
            }
        }
    }

    return result;
}

Circuit SS_algorithm(const Substitution &sub, bool reduction) {
    if (!is_power_of_2(sub.power())) {
        throw SynthException("Substitution size should be power of 2");
    }

    size_t dim = std::log2(sub.power());
    Circuit c(dim);
    Substitution sub_base(sub.power());
    if (sub_base == sub) {
        return c;
    }
    size_t distance_base = cayley_distance(sub, sub_base);

    size_t counter = 0;
    Gate best_gate;
    Substitution best_gate_sub(2);  // we have no default constructor or Subs
    const auto gates_subs = generate_gates_substitutions(dim);
    while (sub_base != sub && counter < 1000) {
        counter++;
        size_t distance_min = distance_base;
        for (const auto &[g, g_sub]: gates_subs) {
            auto current_distance = cayley_distance(sub_base * g_sub, sub);
            if (current_distance < distance_min) {
                best_gate = g;
                best_gate_sub = g_sub;
                distance_min = current_distance;
            }
        }
        if (best_gate.empty()) {
            throw SynthException("Can not chose the best gate");
        }
        c.add(best_gate);
        sub_base *= best_gate_sub;
    }

    if (reduction) {
        c.reduce();
    }

    if (c.produce_mapping() != sub) {
        LOG_DEBUG("The synthesized circuit produces an incorrect mapping", static_cast<std::string>(c));
        throw SynthException("Unable to synthesize Circuit");
    }

    return c;
}
