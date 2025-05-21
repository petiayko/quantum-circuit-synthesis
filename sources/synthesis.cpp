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
    auto bm_bf = bm.coordinate_functions();
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

Circuit RW_algorithm(const BinaryMapping &bm) {
    auto c = Circuit(bm.inputs_number());

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
        return Circuit(sub.power());
    }
    BinaryMapping bm(sub);
    return RW_algorithm(bm);
}
