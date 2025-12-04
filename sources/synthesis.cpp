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
    if (algo == Algo::ZKB) {
        return ZKB_algorithm(bm, reduction);
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
    if (algo == Algo::ZKB) {
        return ZKB_algorithm(sub, reduction);
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

std::vector<controls_type> generate_gate_controls(size_t nest, size_t min_controls, size_t max_controls, size_t dim) {
    if (max_controls < min_controls) {
        return {};
    }

    std::vector<controls_type> result;

    for (size_t controls_mask = 0; controls_mask < static_cast<size_t>(1 << dim); controls_mask++) {
        // controls_mask: 0 - no input; 1 - input
        if (decimal_to_binary_v(controls_mask, dim)[nest]) {
            continue;
        }

        size_t controls_number = static_cast<size_t>(std::popcount(controls_mask));
        if (controls_number > max_controls || controls_number < min_controls) {
            continue;
        }

        const auto controls_mask_binary = decimal_to_binary_v(controls_mask, dim);
        for (size_t inversions_mask = 0;
             inversions_mask < static_cast<size_t>(1 << controls_number); inversions_mask++) {
            result.emplace_back();
            const auto inversions_mask_binary = decimal_to_binary_v(inversions_mask, controls_number);
            size_t control_type_i = 0;
            for (size_t i = 0; i < controls_mask_binary.size(); i++) {
                if (!controls_mask_binary[i]) {
                    continue;
                }
                result.back()[i] = inversions_mask_binary[control_type_i];
                control_type_i++;
            }
        }
    }
    return result;
}

std::vector<Gate> generate_gates_by_type_nest(GateType type, size_t nest, size_t max_controls, size_t dim) {
    // generate ALL possible Gates with
    //      set type
    //      set nest line
    //      set dim
    //      AND with control lines number not gather than max_controls
    if (type == GateType::SWAP || type == GateType::CSWAP) {
        throw GateException("Unable to generate SWAP or CSWAP gates by nest line number");
    }

    std::vector<Gate> result;

    size_t min_controls = 0;
    size_t real_max_controls = 0;
    if (type == GateType::CNOT) {
        min_controls = 1;
        real_max_controls = 1;
    } else if (type == GateType::kCNOT) {
        min_controls = 2;
        real_max_controls = max_controls;
    }

    for (const auto &controls: generate_gate_controls(nest, min_controls, real_max_controls, dim)) {
        result.emplace_back(type, std::vector<size_t>{nest}, controls, dim);
    }

    return result;
}

std::vector<Gate> generate_gates_by_type(GateType type, size_t max_controls, size_t dim) {
    std::vector<Gate> result;

    if (type == GateType::NOT || type == GateType::CNOT || type == GateType::kCNOT) {
        size_t min_controls = 0;
        size_t real_max_controls = 0;
        if (type == GateType::CNOT) {
            min_controls = 1;
            real_max_controls = 1;
        } else if (type == GateType::kCNOT) {
            min_controls = 2;
            real_max_controls = max_controls;
        }

        for (size_t nest = 0; nest < dim; nest++) {
            for (const auto &controls: generate_gate_controls(nest, min_controls, real_max_controls, dim)) {
                result.emplace_back(type, std::vector<size_t>{nest}, controls, dim);
            }
        }
        return result;
    }

    if (type == GateType::SWAP) {
        for (size_t i = 0; i < dim; i++) {
            for (size_t j = i + 1; j < dim; j++) {
                result.emplace_back(type, std::vector<size_t>{i, j}, controls_type(), dim);
            }
        }
        return result;
    }

    for (size_t i = 0; i < dim; i++) {
        for (size_t j = i + 1; j < dim; j++) {
            for (size_t k = 0; k < dim; k++) {
                if (k == i || k == j) {
                    continue;
                }
                result.emplace_back(type, std::vector<size_t>{i, j}, controls_type{{k, true}}, dim);
                result.emplace_back(type, std::vector<size_t>{i, j}, controls_type{{k, false}}, dim);
            }
        }
    }

    return result;
}

std::vector<Gate> generate_all_gates(const std::vector<GateType> &types, size_t dim) {
    if (!dim) {
        throw SynthException("Dimension value should be at least 1");
    }
    std::vector<Gate> result;
    for (const auto type: std::unordered_set<GateType>(types.begin(), types.end())) {
        for (const auto &gate: generate_gates_by_type(type, dim - 1, dim)) {
            result.push_back(gate);
        }
    }

    return result;
}

std::vector<Gate> generate_all_gates(size_t dim) {
    if (!dim) {
        throw SynthException("Dimension value should be at least 1");
    }
    return generate_all_gates({
                                      GateType::NOT,
                                      GateType::CNOT,
                                      GateType::kCNOT,
                                      GateType::SWAP,
                                      GateType::CSWAP,
                              }, dim);
}

std::vector<Gate> generate_all_gates(const std::vector<GateType> &types, size_t nest,
                                     size_t max_controls, size_t dim) {
    if (!dim) {
        throw SynthException("Gates dimension value should be at least 1");
    }
    if (nest > dim - 1) {
        throw SynthException("For Gates generating set nest line should be gather or equal dimension value");
    }
    if (std::find(types.begin(), types.end(), GateType::SWAP) != types.end() ||
        std::find(types.begin(), types.end(), GateType::CSWAP) != types.end()) {
        throw SynthException("Impossible to generate SWAP of CSWAP gates on nest");
    }
    std::vector<Gate> result;
    for (const auto type: std::unordered_set<GateType>(types.begin(), types.end())) {
        for (const auto &gate: generate_gates_by_type_nest(type, nest, max_controls, dim)) {
            result.push_back(gate);
        }
    }

    return result;
}

std::vector<Gate> generate_all_gates(size_t nest, size_t dim) {
    if (!dim) {
        throw SynthException("Gates dimension value should be at least 1");
    }
    if (nest > dim - 1) {
        throw SynthException("For Gates generating set nest line should be gather or equal dimension value");
    }
    return generate_all_gates({
                                      GateType::NOT,
                                      GateType::CNOT,
                                      GateType::kCNOT,
                              }, nest, dim, dim);
}

Circuit RW_algorithm(const BinaryMapping &bm, bool reduction) {
    const size_t MAX_CONTROL_LINES = 3;

    auto bm_extend = bm.extend();
    auto bm_cf = bm_extend.coordinate_functions();
    const auto outputs = bm_extend.outputs_number();

    auto c = Circuit(outputs);

    std::unordered_map<size_t, std::vector<Gate>> precomputed_gates;
    for (size_t i = 0; i < outputs; i++) {
        precomputed_gates[i] = generate_all_gates({GateType::CNOT, GateType::kCNOT}, i, MAX_CONTROL_LINES, outputs);
    }

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
        // TODO act not to bm, act to BF???
        for (const auto &gate: precomputed_gates[nest]) {
            if (gate.type() != GateType::CNOT || gate.nests().front() != nest) {
                continue;
            }
            gate.act(bm_cf);
            auto complexity_new = bm_cf[nest].complexity();
            if (complexity_new - complexity >= max_complexity_diff) {
                max_complexity_diff = complexity_new - complexity;
                best_gate = gate;
            }
            gate.act(bm_cf);
        }
        if (max_complexity_diff) {
            c.insert(best_gate, 0);
            best_gate.act(bm_cf);
            continue;
        }

        // kCNOT 2
        for (const auto &gate: precomputed_gates[nest]) {
            if (gate.controls().size() != 2 || gate.nests().front() != nest) {
                continue;
            }
            gate.act(bm_cf);
            auto complexity_new = bm_cf[nest].complexity();
            if (complexity_new - complexity >= max_complexity_diff) {
                max_complexity_diff = complexity_new - complexity;
                best_gate = gate;
            }
            gate.act(bm_cf);
        }
        if (max_complexity_diff) {
            c.insert(best_gate, 0);
            best_gate.act(bm_cf);
            continue;
        }

        // kCNOT 3
        for (const auto &gate: precomputed_gates[nest]) {
            if (gate.controls().size() != 3 || gate.nests().front() != nest) {
                continue;
            }
            gate.act(bm_cf);
            auto complexity_new = bm_cf[nest].complexity();
            if (complexity_new - complexity >= max_complexity_diff) {
                max_complexity_diff = complexity_new - complexity;
                best_gate = gate;
            }
            gate.act(bm_cf);
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
        std::cout << c;
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
    auto bm_extended = bm.extend();
    auto c = SS_algorithm(Substitution(bm_extended), reduction);
    c.set_memory(bm_extended.inputs_number() - bm.inputs_number());
    return c;
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

    std::unordered_map<Gate, Substitution> gates_substitutions;
    for (const auto &gate: generate_all_gates(dim)) {
        gates_substitutions.insert({gate, gate.act()});
    }

    size_t distance_min = std::numeric_limits<size_t>::max();

    while (sub_base != sub) {
        Gate best_gate;
        for (const auto &[g, g_sub]: gates_substitutions) {
            auto current_distance = cayley_distance(sub_base * g_sub, sub);
            if (current_distance < distance_min) {
                best_gate = g;
                distance_min = current_distance;
            }
        }
        if (best_gate.empty()) {
            throw SynthException("Can not chose the best gate");
        }
        c.add(best_gate);
        sub_base *= best_gate.act();
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

Circuit ZKB_algorithm(const BinaryMapping &bm, bool reduction) {
    auto bm_extended = bm.extend();
    auto c = ZKB_algorithm(Substitution(bm_extended), reduction);
    c.set_memory(bm_extended.inputs_number() - bm.inputs_number());
    return c;
}

Circuit ZKB_algorithm(const Substitution &sub, bool reduction) {
    if (!is_power_of_2(sub.power())) {
        throw SynthException("Substitution size should be power of 2");
    }
    if (sub.power() < 4) {
        throw SynthException("Impossible to apply the ZKB algorithm to a substitution of power less than 16");
    }

    size_t dim = std::log2(sub.power());
    Circuit c(dim);
    if (sub.is_identical()) {
        return c;
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
