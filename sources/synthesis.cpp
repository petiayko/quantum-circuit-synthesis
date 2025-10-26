#include "synthesis.hpp"


Circuit synthesize(const BinaryMapping &bm, const std::string &algo) {
    if (algo == "dummy") {
        return dummy_algorithm(bm);
    } else if (algo == "rw") {
        return RW_algorithm(bm);
    }
    throw SynthException("Unknown synthesis algorithm: " + algo);
}

Circuit synthesize(const Substitution &sub, const std::string &algo) {
    if (algo == "dummy") {
        return dummy_algorithm(sub);
    } else if (algo == "rw") {
        return RW_algorithm(sub);
    }
    throw SynthException("Unknown synthesis algorithm: " + algo);
}

Circuit dummy_algorithm(const BinaryMapping &bm) {
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
                std::vector<control> controls;
                auto numbers = bits_mask(c_set, bm.inputs_number());
                std::transform(numbers.begin(), numbers.end(), std::back_inserter(controls),
                               [](auto num) { return std::make_pair(num, true); });
                c.add(Gate(GateType::kCNOT, {bm.inputs_number() + i}, controls, c_dim));
            }
        }
    }
    return c;
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
        std::vector<control> marked_controls;
        auto mask = decimal_to_binary_v<int>(i, controls.size());
        for (size_t j = 0; j < mask.size(); j++) {
            marked_controls.emplace_back(controls[j], !mask[j]);
        }
        result.emplace_back(type, std::vector<size_t>{nest}, marked_controls, dim);
    }

    return result;
}

Circuit RW_algorithm(const BinaryMapping &bm) {
    auto bm_extend = bm.extend();
    auto bm_cf = bm_extend.coordinate_functions();
    const auto outputs = bm_extend.outputs_number();

    auto c = Circuit(outputs);

    for (int i = outputs - 1;; i--) {
        if (c.produce_mapping() == bm_extend) {
            c.set_memory(bm_extend.inputs_number() - bm.inputs_number());
            c.simplify();
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

    if (c.produce_mapping() != bm_extend) {
        throw SynthException("Unable to synthesize Circuit");
    }

    c.set_memory(bm_extend.inputs_number() - bm.inputs_number());
    c.simplify();
    return c;
}

Circuit dummy_algorithm(const Substitution &sub) {
    if (!is_power_of_2(sub.power())) {
        throw SynthException("Substitution size must be power of 2");
    }
    BinaryMapping bm(sub);
    return dummy_algorithm(bm);
}

Circuit RW_algorithm(const Substitution &sub) {
    if (!is_power_of_2(sub.power())) {
        throw SynthException("Substitution size should be power of 2");
    }
    if (sub.is_identical()) {
        return Circuit(std::log2(sub.power()));
    }
    BinaryMapping bm(sub);
    return RW_algorithm(bm);
}
