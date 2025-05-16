#include <gtest/gtest.h>

#include "gates.hpp"

TEST(Gates, Constructor) {
    EXPECT_THROW(Gate(-1, {}, {}, 2), std::runtime_error);
    EXPECT_THROW(Gate(5, {}, {}, 2), std::runtime_error);
    EXPECT_THROW(Gate(NOT, {4}, {}, 1), std::runtime_error);

    // NOT
    EXPECT_THROW(Gate(NOT, {}, {}, 5), std::runtime_error);
    EXPECT_THROW(Gate(NOT, {}, {1}, 5), std::runtime_error);
    EXPECT_THROW(Gate(NOT, {0}, {1}, 5), std::runtime_error);
    EXPECT_THROW(Gate(NOT, {0, 2}, {}, 5), std::runtime_error);

    //CNOT
    EXPECT_THROW(Gate(CNOT, {}, {1}, 5), std::runtime_error);
    EXPECT_THROW(Gate(CNOT, {1}, {}, 5), std::runtime_error);
    EXPECT_THROW(Gate(CNOT, {1}, {1}, 5), std::runtime_error);
    EXPECT_THROW(Gate(CNOT, {0, 1}, {1}, 5), std::runtime_error);
    EXPECT_THROW(Gate(CNOT, {0}, {1, 2}, 5), std::runtime_error);

    //kCNOT
    EXPECT_THROW(Gate(kCNOT, {}, {1}, 5), std::runtime_error);
    EXPECT_THROW(Gate(kCNOT, {1}, {}, 5), std::runtime_error);
    EXPECT_THROW(Gate(kCNOT, {1}, {1}, 5), std::runtime_error);
    EXPECT_THROW(Gate(kCNOT, {1}, {1, 4}, 5), std::runtime_error);
    EXPECT_THROW(Gate(kCNOT, {0, 1}, {1}, 5), std::runtime_error);
    EXPECT_THROW(Gate(kCNOT, {0}, {1, 1}, 5), std::runtime_error);

    // SWAP
    EXPECT_THROW(Gate(SWAP, {}, {1}, 5), std::runtime_error);
    EXPECT_THROW(Gate(SWAP, {1}, {}, 5), std::runtime_error);
    EXPECT_THROW(Gate(SWAP, {1}, {}, 5), std::runtime_error);
    EXPECT_THROW(Gate(SWAP, {0, 1}, {1}, 5), std::runtime_error);
    EXPECT_THROW(Gate(SWAP, {1, 1}, {}, 5), std::runtime_error);

    // CSWAP
    EXPECT_THROW(Gate(CSWAP, {}, {1}, 5), std::runtime_error);
    EXPECT_THROW(Gate(CSWAP, {1}, {}, 5), std::runtime_error);
    EXPECT_THROW(Gate(CSWAP, {1}, {}, 5), std::runtime_error);
    EXPECT_THROW(Gate(CSWAP, {0, 1}, {1}, 5), std::runtime_error);
    EXPECT_THROW(Gate(CSWAP, {1, 1}, {}, 5), std::runtime_error);
    EXPECT_THROW(Gate(CSWAP, {1, 2}, {}, 5), std::runtime_error);
    EXPECT_THROW(Gate(CSWAP, {1, 2}, {2}, 5), std::runtime_error);
}

TEST(Gates, ConstructorString) {
    EXPECT_THROW(Gate("", 5), std::runtime_error);
    EXPECT_THROW(Gate("gate", 5), std::runtime_error);
    EXPECT_THROW(Gate("CCNOT(3; 2, 1)", 5), std::runtime_error);
    EXPECT_THROW(Gate("NOT(30)", 5), std::runtime_error);

    // NOT
    EXPECT_THROW(Gate("NOT)", 5), std::runtime_error);
    EXPECT_THROW(Gate("NOT(", 5), std::runtime_error);
    EXPECT_THROW(Gate("NOT()", 5), std::runtime_error);
    EXPECT_THROW(Gate("NOT(2", 5), std::runtime_error);
    EXPECT_THROW(Gate("NOT(2;4)", 5), std::runtime_error);
    EXPECT_THROW(Gate("NOT(2,4)", 5), std::runtime_error);
    EXPECT_NO_THROW(Gate("NOT(2)", 5));
    EXPECT_NO_THROW(Gate("  \t   NOT(2)", 5));
    EXPECT_NO_THROW(Gate("  \t   NOT  ( 2         )             ", 5));

    // CNOT
    EXPECT_THROW(Gate("CNOT()", 5), std::runtime_error);
    EXPECT_THROW(Gate("CNOT(2,4)", 5), std::runtime_error);
    EXPECT_TRUE(Gate("cNoT(2;4)", 5) == Gate(CNOT, {2}, {4}, 5));
    EXPECT_TRUE(Gate("  \t   cnot(2;1)", 5) == Gate(CNOT, {2}, {1}, 5));
    EXPECT_TRUE(Gate("  \t   cnOT  ( 2    ;\t3     )             ", 5) == Gate(CNOT, {2}, {3}, 5));

    // kCNOT
    EXPECT_THROW(Gate("kCNOT()", 5), std::runtime_error);
    EXPECT_THROW(Gate("kCNOT(2,4,3)", 5), std::runtime_error);
    EXPECT_THROW(Gate("kCNOT(2,4;3)", 5), std::runtime_error);
    EXPECT_TRUE(Gate("kcnot(2;4, 1)", 5) == Gate(kCNOT, {2}, {4, 1}, 5));
    EXPECT_TRUE(Gate("  \t   Kcnot(2;1,0)", 5) == Gate(kCNOT, {2}, {1, 0}, 5));
    EXPECT_TRUE(Gate("  \t   kCnOt  ( 2    ;\t3   ,  4  )             ", 5) == Gate(kCNOT, {2}, {3, 4}, 5));

    // SWAP
    EXPECT_THROW(Gate("SWAP()", 5), std::runtime_error);
    EXPECT_THROW(Gate("SWAP(2,4;)", 5), std::runtime_error);
    EXPECT_THROW(Gate("SWAP(2;4)", 5), std::runtime_error);
    EXPECT_TRUE(Gate("swap(2,1)", 5) == Gate(SWAP, {2, 1}, {}, 5));
    EXPECT_TRUE(Gate("  \t   SWap(2,4)", 5) == Gate(SWAP, {2, 4}, {}, 5));
    EXPECT_TRUE(Gate("  \t   SwAp  ( 2    \t   ,  4  )             ", 5) == Gate(SWAP, {2, 4}, {}, 5));

    // CSWAP
    EXPECT_THROW(Gate("CSWAP()", 5), std::runtime_error);
    EXPECT_THROW(Gate("CSWAP(2,4;)", 5), std::runtime_error);
    EXPECT_THROW(Gate("CSWAP(2,0;4,1)", 5), std::runtime_error);
    EXPECT_TRUE(Gate("cswap(2,1;0)", 5) == Gate(CSWAP, {2, 1}, {0}, 5));
    EXPECT_TRUE(Gate("  \t   cSWAP(0,1;2)", 5) == Gate(CSWAP, {0, 1}, {2}, 5));
    EXPECT_TRUE(Gate("  \t   CswaP  ( 2   ,3 \t   ;\t\t  4  )             ", 5) == Gate(CSWAP, {2, 3}, {4}, 5));
}

TEST(Gates, ActNOT) {
    auto g1 = Gate("NOT(3)", 5);
    auto g2 = Gate("NOT(0)", 5);
    auto g3 = Gate("NOT(1)", 5);
    auto g4 = Gate("NOT(2)", 5);
    auto g5 = Gate("NOT(4)", 5);

    std::vector<bool> vector1({1, 0, 0});
    EXPECT_THROW(g1.act(vector1), std::runtime_error);

    std::vector<bool> vector2({1, 0, 0, 1, 1});
    g1.act(vector2);
    EXPECT_EQ(vector2, (std::vector<bool>{1, 0, 0, 0, 1}));
    g1.act(vector2);
    EXPECT_EQ(vector2, (std::vector<bool>{1, 0, 0, 1, 1}));

    g1.act(vector2);
    g2.act(vector2);
    g3.act(vector2);
    g4.act(vector2);
    g5.act(vector2);
    EXPECT_EQ(vector2, (std::vector<bool>{0, 1, 1, 0, 0}));

    // Act for BF
    auto g6 = Gate("NOT(1)", 3);
    auto g7 = Gate("NOT(2)", 3);
    auto g8 = Gate("NOT(0)", 3);

    std::vector<BooleanFunction> bf_vector1(
            {BooleanFunction("00000000"), BooleanFunction("11010101")});
    EXPECT_THROW(g6.act(bf_vector1), std::runtime_error);

    std::vector<BooleanFunction> bf_vector2(
            {BooleanFunction("00000000"), BooleanFunction("11010101"), BooleanFunction("1101")});
    EXPECT_THROW(g6.act(bf_vector2), std::runtime_error);

    std::vector<BooleanFunction> bf_vector3(
            {BooleanFunction("00000000"), BooleanFunction("11010101"), BooleanFunction("11111110")});
    g6.act(bf_vector3);
    EXPECT_EQ(bf_vector3, (std::vector<BooleanFunction>{BooleanFunction("00000000"), BooleanFunction("00101010"),
                                                        BooleanFunction("11111110")}));

    g6.act(bf_vector3);
    g7.act(bf_vector3);
    g8.act(bf_vector3);
    EXPECT_EQ(bf_vector3, (std::vector<BooleanFunction>{BooleanFunction("11111111"), BooleanFunction("11010101"),
                                                        BooleanFunction("00000001")}));
}

TEST(Gates, ActCNOT) {
    auto g1 = Gate("CNOT(2;1)", 5);
    auto g2 = Gate("CNOT(0;4)", 5);
    auto g3 = Gate("CNOT(3;2)", 5);
    auto g4 = Gate("CNOT(4;0)", 5);
    auto g5 = Gate("CNOT(1;3)", 5);

    std::vector<bool> vector1({1, 0, 0});
    EXPECT_THROW(g1.act(vector1), std::runtime_error);

    std::vector<bool> vector2({1, 0, 0, 1, 1});
    g1.act(vector2);
    EXPECT_EQ(vector2, (std::vector<bool>{1, 0, 0, 1, 1}));

    g1.act(vector2);
    g2.act(vector2);
    g3.act(vector2);
    g4.act(vector2);
    g5.act(vector2);
    EXPECT_EQ(vector2, (std::vector<bool>{0, 1, 0, 1, 1}));

    // Act for BF
    auto g6 = Gate("CNOT(1; 2)", 3);
    auto g7 = Gate("CNOT(2; 1)", 3);
    auto g8 = Gate("CNOT(0; 1)", 3);

    std::vector<BooleanFunction> bf_vector1(
            {BooleanFunction("00000000"), BooleanFunction("11010101")});
    EXPECT_THROW(g6.act(bf_vector1), std::runtime_error);

    std::vector<BooleanFunction> bf_vector2(
            {BooleanFunction("00000000"), BooleanFunction("11010101"), BooleanFunction("1101")});
    EXPECT_THROW(g6.act(bf_vector2), std::runtime_error);

    std::vector<BooleanFunction> bf_vector3(
            {BooleanFunction("00000000"), BooleanFunction("11010101"), BooleanFunction("11111110")});
    g6.act(bf_vector3);
    EXPECT_EQ(bf_vector3, (std::vector<BooleanFunction>{BooleanFunction("00000000"), BooleanFunction("00101011"),
                                                        BooleanFunction("11111110")}));

    g6.act(bf_vector3);
    g7.act(bf_vector3);
    g8.act(bf_vector3);
    EXPECT_EQ(bf_vector3, (std::vector<BooleanFunction>{BooleanFunction("11010101"), BooleanFunction("11010101"),
                                                        BooleanFunction("00101011")}));
}

TEST(Gates, ActkCNOT) {
    auto g1 = Gate("kCNOT(2;0,3,4)", 5);
    auto g2 = Gate("kCNOT(0;4)", 5);
    auto g3 = Gate("kCNOT(3;2,1)", 5);
    auto g4 = Gate("kCNOT(4;0,3,1)", 5);
    auto g5 = Gate("kCNOT(1;3,2,4)", 5);

    std::vector<bool> vector1({1, 0, 0});
    EXPECT_THROW(g1.act(vector1), std::runtime_error);

    std::vector<bool> vector2({1, 0, 0, 1, 1});
    g1.act(vector2);
    EXPECT_EQ(vector2, (std::vector<bool>{1, 0, 1, 1, 1}));

    g1.act(vector2);
    g2.act(vector2);
    g3.act(vector2);
    g4.act(vector2);
    g5.act(vector2);
    EXPECT_EQ(vector2, (std::vector<bool>{0, 0, 0, 1, 1}));

    // Act for BF
    auto g6 = Gate("kCNOT(1; 2, 0)", 3);
    auto g7 = Gate("kCNOT(2; 1, 0)", 3);
    auto g8 = Gate("kCNOT(0; 1, 2)", 3);

    std::vector<BooleanFunction> bf_vector1(
            {BooleanFunction("00000000"), BooleanFunction("11010101")});
    EXPECT_THROW(g6.act(bf_vector1), std::runtime_error);

    std::vector<BooleanFunction> bf_vector2(
            {BooleanFunction("00000000"), BooleanFunction("11010101"), BooleanFunction("1101")});
    EXPECT_THROW(g6.act(bf_vector2), std::runtime_error);

    std::vector<BooleanFunction> bf_vector3(
            {BooleanFunction("00101010"), BooleanFunction("11010101"), BooleanFunction("11111110")});
    g6.act(bf_vector3);
    EXPECT_EQ(bf_vector3, (std::vector<BooleanFunction>{BooleanFunction("00101010"), BooleanFunction("11111111"),
                                                        BooleanFunction("11111110")}));

    g6.act(bf_vector3);
    g7.act(bf_vector3);
    g8.act(bf_vector3);
    EXPECT_EQ(bf_vector3, (std::vector<BooleanFunction>{BooleanFunction("11111110"), BooleanFunction("11010101"),
                                                        BooleanFunction("11111110")}));
}

TEST(Gates, ActSWAP) {
    auto g1 = Gate("SWAP(2,0)", 5);
    auto g2 = Gate("SWAP(0,4)", 5);
    auto g3 = Gate("SWAP(3,2)", 5);
    auto g4 = Gate("SWAP(4,0)", 5);
    auto g5 = Gate("SWAP(1,3)", 5);

    std::vector<bool> vector1({1, 0, 0});
    EXPECT_THROW(g1.act(vector1), std::runtime_error);

    std::vector<bool> vector2({1, 0, 0, 1, 1});
    g1.act(vector2);
    EXPECT_EQ(vector2, (std::vector<bool>{0, 0, 1, 1, 1}));

    g1.act(vector2);
    g2.act(vector2);
    g3.act(vector2);
    g4.act(vector2);
    g5.act(vector2);
    EXPECT_EQ(vector2, (std::vector<bool>{1, 0, 1, 0, 1}));

    // Act for BF
    auto g6 = Gate("SWAP(1,2)", 3);
    auto g7 = Gate("SWAP(0,1)", 3);
    auto g8 = Gate("SWAP(0,2)", 3);

    std::vector<BooleanFunction> bf_vector1(
            {BooleanFunction("00000000"), BooleanFunction("11010101")});
    EXPECT_THROW(g6.act(bf_vector1), std::runtime_error);

    std::vector<BooleanFunction> bf_vector2(
            {BooleanFunction("00000000"), BooleanFunction("11010101"), BooleanFunction("1101")});
    EXPECT_THROW(g6.act(bf_vector2), std::runtime_error);

    std::vector<BooleanFunction> bf_vector3(
            {BooleanFunction("00101010"), BooleanFunction("11010101"), BooleanFunction("11111110")});
    g6.act(bf_vector3);
    EXPECT_EQ(bf_vector3, (std::vector<BooleanFunction>{BooleanFunction("00101010"), BooleanFunction("11111110"),
                                                        BooleanFunction("11010101")}));

    g6.act(bf_vector3);
    g7.act(bf_vector3);
    g8.act(bf_vector3);
    EXPECT_EQ(bf_vector3, (std::vector<BooleanFunction>{BooleanFunction("11111110"), BooleanFunction("00101010"),
                                                        BooleanFunction("11010101")}));
}

TEST(Gates, ActCSWAP) {
    auto g1 = Gate("CSWAP(2,0;4)", 5);
    auto g2 = Gate("CSWAP(0,4;1)", 5);
    auto g3 = Gate("CSWAP(3,2;4)", 5);
    auto g4 = Gate("CSWAP(4,0;2)", 5);
    auto g5 = Gate("CSWAP(1,2;0)", 5);

    std::vector<bool> vector1({1, 0, 0});
    EXPECT_THROW(g1.act(vector1), std::runtime_error);

    std::vector<bool> vector2({1, 0, 0, 1, 1});
    g1.act(vector2);
    EXPECT_EQ(vector2, (std::vector<bool>{0, 0, 1, 1, 1}));

    g1.act(vector2);
    g2.act(vector2);
    g3.act(vector2);
    g4.act(vector2);
    g5.act(vector2);
    EXPECT_EQ(vector2, (std::vector<bool>{1, 1, 0, 0, 1}));

    // Act for BF
    auto g6 = Gate("CSWAP(1,2;0)", 3);
    auto g7 = Gate("CSWAP(0,1;2)", 3);
    auto g8 = Gate("CSWAP(0,2;1)", 3);

    std::vector<BooleanFunction> bf_vector1(
            {BooleanFunction("00000000"), BooleanFunction("11010101")});
    EXPECT_THROW(g6.act(bf_vector1), std::runtime_error);

    std::vector<BooleanFunction> bf_vector2(
            {BooleanFunction("00000000"), BooleanFunction("11010101"), BooleanFunction("1101")});
    EXPECT_THROW(g6.act(bf_vector2), std::runtime_error);

    std::vector<BooleanFunction> bf_vector3(
            {BooleanFunction("00101010"), BooleanFunction("11010101"), BooleanFunction("11111110")});
    g6.act(bf_vector3);
    EXPECT_EQ(bf_vector3, (std::vector<BooleanFunction>{BooleanFunction("00101010"), BooleanFunction("11111111"),
                                                        BooleanFunction("11010100")}));

    g6.act(bf_vector3);
    g7.act(bf_vector3);
    g8.act(bf_vector3);
    EXPECT_EQ(bf_vector3, (std::vector<BooleanFunction>{BooleanFunction("11111110"), BooleanFunction("00101011"),
                                                        BooleanFunction("11010100")}));
}

TEST(Gates, Stream) {
    std::stringstream out_stream;

    out_stream << Gate(NOT, {29415}, {}, 100000);
    EXPECT_EQ(out_stream.str(), "NOT(29415)");
    out_stream.str("");

    out_stream << Gate(kCNOT, {29415}, {325, 532, 1, 5}, 100000);
    EXPECT_EQ(out_stream.str(), "kCNOT(29415; 325, 532, 1, 5)");
    out_stream.str("");

    out_stream << Gate(CSWAP, {29415, 0}, {99999}, 100000);
    EXPECT_EQ(out_stream.str(), "CSWAP(29415, 0; 99999)");
    out_stream.str("");
}
