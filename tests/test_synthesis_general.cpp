#include <gtest/gtest.h>
#include <format>

#include "synthesis.hpp"

static const size_t MAX_DIM = 9;


TEST(Synthesis, General) {
    Substitution sub("0 1");
    EXPECT_THROW(synthesize(sub, Algo::UNKNOWN), SynthException);
    EXPECT_THROW(synthesize(sub, Algo::EMPTY), SynthException);
    EXPECT_NO_THROW(synthesize(sub));

    BinaryMapping bm("0 1\n1 0");
    EXPECT_THROW(synthesize(bm, Algo::UNKNOWN), SynthException);
    EXPECT_THROW(synthesize(bm, Algo::EMPTY), SynthException);
    EXPECT_NO_THROW(synthesize(bm));
}

template<typename T>
bool contains(const std::vector<T> &vec, const T &value) {
    return std::find(vec.begin(), vec.end(), value) != vec.end();
}

size_t count_gates(GateType type, size_t dim, bool on_nest = false) noexcept {
    size_t number = 0;
    if (type == GateType::NOT) {
        number = 1;
    } else if (type == GateType::CNOT) {
        if (dim < 2) {
            return 0;
        }
        number = 2 * (dim - 1);
    } else if (type == GateType::kCNOT) {
        if (dim < 3) {
            return 0;
        }
        number = std::pow(3, dim - 1) - 2 * dim + 1;
    } else if (type == GateType::SWAP) {
        if (dim < 2) {
            return 0;
        }
        return dim * (dim - 1) / 2;
    } else if (type == GateType::CSWAP) {
        if (dim < 3) {
            return 0;
        }
        return dim * (dim - 1) * (dim - 2);
    } else {
        return -1;
    }
    if (on_nest) {
        return number;
    }
    return number * dim;
}


TEST(Synthesis, AllGatesGenerator) {
    EXPECT_THROW(generate_all_gates(0), SynthException);

    auto gates = generate_all_gates(1);
    EXPECT_EQ(gates, generate_all_gates({GateType::NOT}, 1));
    EXPECT_EQ(gates, generate_all_gates({GateType::NOT, GateType::CNOT}, 1));
    EXPECT_EQ(gates, generate_all_gates({GateType::NOT, GateType::CNOT, GateType::kCNOT}, 1));
    EXPECT_EQ(gates, generate_all_gates({GateType::NOT, GateType::CNOT, GateType::kCNOT, GateType::SWAP}, 1));
    EXPECT_EQ(gates,
              generate_all_gates({GateType::NOT, GateType::CNOT, GateType::kCNOT, GateType::SWAP, GateType::CSWAP}, 1));
    EXPECT_EQ(gates.size(), 1);
    EXPECT_TRUE(contains(gates, Gate("NOT(0)", 1)));

    gates = generate_all_gates(2);
    EXPECT_EQ(gates, generate_all_gates({GateType::NOT, GateType::CNOT, GateType::SWAP}, 2));
    EXPECT_EQ(gates.size(), 7);
    EXPECT_TRUE(contains(gates, Gate("NOT(0)", 2)));
    EXPECT_TRUE(contains(gates, Gate("NOT(1)", 2)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(0; 1)", 2)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(0; !1)", 2)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(1; 0)", 2)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(1; !0)", 2)));
    EXPECT_TRUE(contains(gates, Gate("SWAP(0, 1)", 2)));

    gates = generate_all_gates(3);
    EXPECT_EQ(gates.size(), 36);
    EXPECT_TRUE(contains(gates, Gate("NOT(0)", 3)));
    EXPECT_TRUE(contains(gates, Gate("NOT(1)", 3)));
    EXPECT_TRUE(contains(gates, Gate("NOT(2)", 3)));
    EXPECT_TRUE(contains(gates, Gate("SWAP(0, 1)", 3)));
    EXPECT_TRUE(contains(gates, Gate("SWAP(0, 2)", 3)));
    EXPECT_TRUE(contains(gates, Gate("SWAP(1, 2)", 3)));
    EXPECT_TRUE(contains(gates, Gate("CSWAP(0, 1; 2)", 3)));
    EXPECT_TRUE(contains(gates, Gate("CSWAP(0, 1; !2)", 3)));
    EXPECT_TRUE(contains(gates, Gate("CSWAP(0, 2; 1)", 3)));
    EXPECT_TRUE(contains(gates, Gate("CSWAP(0, 2; !1)", 3)));
    EXPECT_TRUE(contains(gates, Gate("CSWAP(1, 2; 0)", 3)));
    EXPECT_TRUE(contains(gates, Gate("CSWAP(1, 2; !0)", 3)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(0; 1)", 3)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(0; !1)", 3)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(0; 2)", 3)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(0; !2)", 3)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(1; 0)", 3)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(1; !0)", 3)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(1; 2)", 3)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(1; !2)", 3)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(2; 0)", 3)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(2; !0)", 3)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(2; 1)", 3)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(2; !1)", 3)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(0; 1, 2)", 3)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(0; 1, !2)", 3)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(0; !1, 2)", 3)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(0; !1, !2)", 3)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(1; 0, 2)", 3)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(1; 0, !2)", 3)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(1; !0, 2)", 3)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(1; !0, !2)", 3)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(2; 0, 1)", 3)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(2; 0, !1)", 3)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(2; !0, 1)", 3)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(2; !0, !1)", 3)));

    for (size_t dim = 1; dim < MAX_DIM; dim++) {
        for (auto type: {GateType::NOT, GateType::CNOT, GateType::kCNOT, GateType::SWAP, GateType::CSWAP}) {
            const std::string msg = std::format("Dim={}, type={} ", dim, static_cast<size_t>(type));
            gates = generate_all_gates({type}, dim);
            EXPECT_EQ(gates.size(), count_gates(type, dim)) << msg;
            EXPECT_EQ(std::unordered_set(gates.begin(), gates.end()).size(), gates.size()) << msg;
            for (const auto &gate: gates) {
                EXPECT_EQ(gate.type(), type) << msg << gate;
            }
        }
    }
}

TEST(Synthesis, NestGatesGenerator) {
    EXPECT_THROW(generate_all_gates(0, 0), SynthException);
    EXPECT_THROW(generate_all_gates(3, 2), SynthException);
    EXPECT_THROW(generate_all_gates(4, 4), SynthException);
    EXPECT_THROW(generate_all_gates({GateType::SWAP}, 2, 4, 4), SynthException);
    EXPECT_THROW(generate_all_gates({GateType::CSWAP}, 2, 4, 4), SynthException);

    auto gates = generate_all_gates(0, 1);
    EXPECT_EQ(gates.size(), 1);
    EXPECT_TRUE(contains(gates, Gate("NOT(0)", 1)));

    gates = generate_all_gates(0, 2);
    EXPECT_EQ(gates.size(), 3);
    EXPECT_TRUE(contains(gates, Gate("NOT(0)", 2)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(0; 1)", 2)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(0; !1)", 2)));

    gates = generate_all_gates(1, 2);
    EXPECT_EQ(gates.size(), 3);
    EXPECT_TRUE(contains(gates, Gate("NOT(1)", 2)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(1; 0)", 2)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(1; !0)", 2)));

    gates = generate_all_gates(0, 3);
    EXPECT_EQ(gates.size(), 9);
    EXPECT_TRUE(contains(gates, Gate("NOT(0)", 3)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(0; 1)", 3)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(0; !1)", 3)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(0; 2)", 3)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(0; !2)", 3)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(0; 1, 2)", 3)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(0; 1, !2)", 3)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(0; !1, 2)", 3)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(0; !1, !2)", 3)));

    gates = generate_all_gates(1, 3);
    EXPECT_EQ(gates.size(), 9);
    EXPECT_TRUE(contains(gates, Gate("NOT(1)", 3)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(1; 0)", 3)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(1; !0)", 3)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(1; 2)", 3)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(1; !2)", 3)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(1; 0, 2)", 3)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(1; 0, !2)", 3)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(1; !0, 2)", 3)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(1; !0, !2)", 3)));

    gates = generate_all_gates(2, 3);
    EXPECT_EQ(gates.size(), 9);
    EXPECT_TRUE(contains(gates, Gate("NOT(2)", 3)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(2; 1)", 3)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(2; !1)", 3)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(2; 0)", 3)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(2; !0)", 3)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(2; 1, 0)", 3)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(2; 1, !0)", 3)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(2; !1, 0)", 3)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(2; !1, !0)", 3)));

    for (size_t dim = 1; dim < MAX_DIM; dim++) {
        for (auto type: {GateType::NOT, GateType::CNOT, GateType::kCNOT}) {
            for (size_t nest = 0; nest < dim; nest++) {
                const std::string msg = std::format("Dim={}, type={}, nest={} ", dim, static_cast<size_t>(type), nest);
                gates = generate_all_gates({type}, nest, dim, dim);
                EXPECT_EQ(gates.size(), count_gates(type, dim, true)) << msg;
                EXPECT_EQ(std::unordered_set(gates.begin(), gates.end()).size(), gates.size()) << msg;
                for (const auto &gate: gates) {
                    EXPECT_EQ(gate.nests(), std::vector<size_t>({nest})) << msg << gate;
                    EXPECT_EQ(gate.type(), type) << msg << gate;
                }
            }
        }
    }

    gates = generate_all_gates({GateType::kCNOT}, 2, 1, 3);
    EXPECT_EQ(gates.size(), 0);

    gates = generate_all_gates({GateType::CNOT, GateType::kCNOT}, 2, 1, 3);
    EXPECT_EQ(gates.size(), 4);
    EXPECT_TRUE(contains(gates, Gate("CNOT(2; 1)", 3)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(2; !1)", 3)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(2; 0)", 3)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(2; !0)", 3)));

    gates = generate_all_gates({GateType::CNOT, GateType::kCNOT}, 2, 2, 4);
    EXPECT_EQ(gates.size(), 18);
    EXPECT_TRUE(contains(gates, Gate("CNOT(2; 0)", 4)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(2; !0)", 4)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(2; 1)", 4)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(2; !1)", 4)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(2; 3)", 4)));
    EXPECT_TRUE(contains(gates, Gate("CNOT(2; !3)", 4)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(2; 0, 1)", 4)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(2; 0, !1)", 4)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(2; !0, 1)", 4)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(2; !0, !1)", 4)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(2; 0, 3)", 4)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(2; 0, !3)", 4)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(2; !0, 3)", 4)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(2; !0, !3)", 4)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(2; 1, 3)", 4)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(2; 1, !3)", 4)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(2; !1, 3)", 4)));
    EXPECT_TRUE(contains(gates, Gate("kCNOT(2; !1, !3)", 4)));
}
