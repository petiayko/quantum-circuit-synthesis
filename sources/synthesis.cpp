#include "synthesis.hpp"


Circuit synthesize(const BinaryMapping &bm, const std::string &algo) {
    if (algo == "dummy") {
        return dummy_algorithm(bm);
    } else if (algo == "rw") {
        return RW_algorithm(bm.extend());
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
                c.add(Gate(GateType::kCNOT, {bm.inputs_number() + i}, bits_mask(c_set, bm.inputs_number()), c_dim));
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

Circuit RW_algorithm(const BinaryMapping &bm) {
    auto bm_extend = bm.extend();
    auto bm_cf = bm_extend.coordinate_functions();
    const auto outputs = bm_extend.outputs_number();

    std::vector<std::vector<int>> bm_spectrum;
    bm_spectrum.resize(bm_extend.outputs_number());
    std::transform(bm_cf.cbegin(), bm_cf.cend(), bm_spectrum.begin(), [](const BooleanFunction &bf) {
        return bf.RW_spectrum();
    });

    auto c = Circuit(bm.inputs_number());
    c.set_memory(bm_extend.inputs_number() - bm.inputs_number());

    bool status = true;
    while (true) {
        if (c.produce_mapping() == bm_extend) {
            break;
        }
        size_t i = 0;
        for (; i < outputs; i++) {
            // CNOT
            std::vector<size_t> best_controls;
            int max_complexity = 0;
            for (const auto &controls: generate_controls(1, outputs, i)) {
                auto g = Gate(GateType::CNOT, {i}, controls, outputs);
                auto complexity = bm_cf[i].complexity();
                g.act(bm_cf);
                auto complexity_new = bm_cf[i].complexity();
                if (complexity_new - complexity > max_complexity) {
                    max_complexity = complexity_new - complexity;
                    best_controls = controls;
                }
                g.act(bm_cf);
            }
            if (max_complexity) {
                auto g = Gate(GateType::CNOT, {i}, best_controls, outputs);
                c.add(g);
                g.act(bm_cf);
                continue;
            }

            // kCNOT 2
            for (const auto &controls: generate_controls(2, outputs, i)) {
                auto g = Gate(GateType::kCNOT, {i}, controls, outputs);
                auto complexity = bm_cf[i].complexity();
                g.act(bm_cf);
                auto complexity_new = bm_cf[i].complexity();
                if (complexity_new - complexity > max_complexity) {
                    max_complexity = complexity_new - complexity;
                    best_controls = controls;
                }
                g.act(bm_cf);
            }
            if (max_complexity) {
                auto g = Gate(GateType::kCNOT, {i}, best_controls, outputs);
                c.add(g);
                g.act(bm_cf);
                continue;
            }

            // kCNOT 3
            for (const auto &controls: generate_controls(3, outputs, i)) {
                auto g = Gate(GateType::kCNOT, {i}, controls, outputs);
                auto complexity = bm_cf[i].complexity();
                g.act(bm_cf);
                auto complexity_new = bm_cf[i].complexity();
                if (complexity_new - complexity > max_complexity) {
                    max_complexity = complexity_new - complexity;
                    best_controls = controls;
                }
                g.act(bm_cf);
            }
            if (max_complexity) {
                auto g = Gate(GateType::kCNOT, {i}, best_controls, outputs);
                c.add(g);
                g.act(bm_cf);
                continue;
            }

            // else
            status = false;
            break;
        }
        if (!status) {
            throw SynthException("Unable to synthesize Circuit");
        }
    }

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
