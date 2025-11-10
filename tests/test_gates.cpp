#include <gtest/gtest.h>

#include "gates.hpp"


TEST(Gates, Constructor) {
    EXPECT_THROW(Gate(GateType(-1), {}, {}, 2), GateException);
    EXPECT_THROW(Gate(GateType(0), {}, {}, 2), GateException);
    EXPECT_THROW(Gate(GateType(5), {}, {}, 2), GateException);
    EXPECT_THROW(Gate(GateType::NOT, {4}, {}, 1), GateException);

    // NOT
    EXPECT_THROW(Gate(GateType::NOT, {}, {}, 5), GateException);
    EXPECT_THROW(Gate(GateType::NOT, {}, {{1, true}}, 5), GateException);
    EXPECT_THROW(Gate(GateType::NOT, {0}, {{1, true}}, 5), GateException);
    EXPECT_THROW(Gate(GateType::NOT, {0, 2}, {}, 5), GateException);

    //CNOT
    EXPECT_THROW(Gate(GateType::CNOT, {}, {{1, true}}, 5), GateException);
    EXPECT_THROW(Gate(GateType::CNOT, {1}, {}, 5), GateException);
    EXPECT_THROW(Gate(GateType::CNOT, {1}, {{1, true}}, 5), GateException);
    EXPECT_THROW(Gate(GateType::CNOT, {0, 1}, {{1, true}}, 5), GateException);
    EXPECT_THROW(Gate(GateType::CNOT, {0}, {{1, true},
                                            {2, true}}, 5), GateException);

    //kCNOT
    EXPECT_THROW(Gate(GateType::kCNOT, {}, {{1, false}}, 5), GateException);
    EXPECT_THROW(Gate(GateType::kCNOT, {1}, {}, 5), GateException);
    EXPECT_THROW(Gate(GateType::kCNOT, {1}, {{1, true}}, 5), GateException);
    EXPECT_THROW(Gate(GateType::kCNOT, {1}, {{1, false},
                                             {4, true}}, 5), GateException);
    EXPECT_THROW(Gate(GateType::kCNOT, {0, 1}, {{1, false}}, 5), GateException);
    EXPECT_THROW(Gate(GateType::kCNOT, {0}, {{1, true},
                                             {1, true}}, 5), GateException);
    EXPECT_THROW(Gate(GateType::kCNOT, {0}, {{1, true},
                                             {1, false}}, 5), GateException);

    // SWAP
    EXPECT_THROW(Gate(GateType::SWAP, {}, {{1, true}}, 5), GateException);
    EXPECT_THROW(Gate(GateType::SWAP, {1}, {}, 5), GateException);
    EXPECT_THROW(Gate(GateType::SWAP, {1}, {}, 5), GateException);
    EXPECT_THROW(Gate(GateType::SWAP, {0, 1}, {{1, false}}, 5), GateException);
    EXPECT_THROW(Gate(GateType::SWAP, {1, 1}, {}, 5), GateException);

    // CSWAP
    EXPECT_THROW(Gate(GateType::CSWAP, {}, {{1, true}}, 5), GateException);
    EXPECT_THROW(Gate(GateType::CSWAP, {1}, {}, 5), GateException);
    EXPECT_THROW(Gate(GateType::CSWAP, {1}, {}, 5), GateException);
    EXPECT_THROW(Gate(GateType::CSWAP, {0, 1}, {{1, true}}, 5), GateException);
    EXPECT_THROW(Gate(GateType::CSWAP, {1, 1}, {}, 5), GateException);
    EXPECT_THROW(Gate(GateType::CSWAP, {1, 2}, {}, 5), GateException);
    EXPECT_THROW(Gate(GateType::CSWAP, {1, 2}, {{2, true}}, 5), GateException);
}

TEST(Gates, ConstructorString) {
    EXPECT_THROW(Gate("", 5), GateException);
    EXPECT_THROW(Gate("gate", 5), GateException);
    EXPECT_THROW(Gate("CCNOT(3; 2, 1)", 5), GateException);
    EXPECT_THROW(Gate("NOT(30)", 5), GateException);

    // NOT
    EXPECT_THROW(Gate("NOT)", 5), GateException);
    EXPECT_THROW(Gate("NOT(", 5), GateException);
    EXPECT_THROW(Gate("NOT()", 5), GateException);
    EXPECT_THROW(Gate("NOT(2", 5), GateException);
    EXPECT_THROW(Gate("NOT(2;4)", 5), GateException);
    EXPECT_THROW(Gate("NOT(2,4)", 5), GateException);
    EXPECT_THROW(Gate("NOT(!2)", 5), StringException);
    EXPECT_TRUE(Gate("NOT(2)", 5) == Gate(GateType::NOT, {2}, {}, 5));
    EXPECT_TRUE(Gate("  \t   NOT(2)", 5) == Gate(GateType::NOT, {2}, {}, 5));
    EXPECT_TRUE(Gate("  \t   NOT  ( 2         )             ", 5) == Gate(GateType::NOT, {2}, {}, 5));
    EXPECT_TRUE(Gate("  \t   NOT  ( 0x2         )             ", 5) == Gate(GateType::NOT, {2}, {}, 5));

    // CNOT
    EXPECT_THROW(Gate("CNOT()", 5), GateException);
    EXPECT_THROW(Gate("CNOT(2,4)", 5), GateException);
    EXPECT_THROW(Gate("CNOT(2;!!4)", 5), StringException);
    EXPECT_THROW(Gate("CNOT(!2;!4)", 5), StringException);
    EXPECT_TRUE(Gate("cNoT(2;4)", 5) == Gate(GateType::CNOT, {2}, {{4, true}}, 5));
    EXPECT_TRUE(Gate("  \t   cnot(2;1)", 5) == Gate(GateType::CNOT, {2}, {{1, true}}, 5));
    EXPECT_TRUE(Gate("  \t   cnOT  ( 2    ;\t3     )             ", 5) == Gate(GateType::CNOT, {2}, {{3, true}}, 5));
    EXPECT_TRUE(Gate("  \t   cnOT  ( 2    ;\t!3     )             ", 5) == Gate(GateType::CNOT, {2}, {{3, false}}, 5));
    EXPECT_TRUE(Gate("  \t   cnOT  ( 2    ;\t0x3     )             ", 5) == Gate(GateType::CNOT, {2}, {{3, true}}, 5));
    EXPECT_TRUE(
            Gate("  \t   cnOT  ( 2    ;\t!0x3     )             ", 5) == Gate(GateType::CNOT, {2}, {{3, false}}, 5));

    // kCNOT
    EXPECT_THROW(Gate("kCNOT()", 5), GateException);
    EXPECT_THROW(Gate("kCNOT(2,4,3)", 5), GateException);
    EXPECT_THROW(Gate("kCNOT(2,4;3)", 5), GateException);
    EXPECT_THROW(Gate("kCNOT(2;4,!!3)", 5), StringException);
    EXPECT_THROW(Gate("kCNOT(!2;4,!3)", 5), StringException);
    EXPECT_TRUE(Gate("kcnot(2;4, 1)", 5) == Gate(GateType::kCNOT, {2}, {{4, true},
                                                                        {1, true}}, 5));
    EXPECT_TRUE(Gate("kcnot(2;!4, !1)", 5) == Gate(GateType::kCNOT, {2}, {{4, false},
                                                                          {1, false}}, 5));
    EXPECT_TRUE(Gate("  \t   Kcnot(2;1,0)", 5) == Gate(GateType::kCNOT, {2}, {{1, true},
                                                                              {0, true}}, 5));
    EXPECT_TRUE(Gate("  \t   Kcnot(2;1,!0)", 5) == Gate(GateType::kCNOT, {2}, {{1, true},
                                                                               {0, false}}, 5));
    EXPECT_TRUE(Gate("  \t   kCnOt  ( 2    ;\t3   ,  4  )             ", 5) ==
                Gate(GateType::kCNOT, {2}, {{3, true},
                                            {4, true}}, 5));
    EXPECT_TRUE(Gate("  \t   kCnOt  ( 2    ;\t3   ,  0x4  )             ", 5) ==
                Gate(GateType::kCNOT, {2}, {{3, true},
                                            {4, true}}, 5));
    EXPECT_TRUE(Gate("  \t   kCnOt  ( 2    ;\t!3   ,  !0x4  )             ", 5) ==
                Gate(GateType::kCNOT, {2}, {{3, false},
                                            {4, false}}, 5));

    // SWAP
    EXPECT_THROW(Gate("SWAP()", 5), GateException);
    EXPECT_THROW(Gate("SWAP(2,4;)", 5), GateException);
    EXPECT_THROW(Gate("SWAP(2;4)", 5), GateException);
    EXPECT_THROW(Gate("SWAP(2,!4)", 5), StringException);
    EXPECT_THROW(Gate("SWAP(!2,4)", 5), StringException);
    EXPECT_TRUE(Gate("swap(2,1)", 5) == Gate(GateType::SWAP, {2, 1}, {}, 5));
    EXPECT_TRUE(Gate("  \t   SWap(2,4)", 5) == Gate(GateType::SWAP, {2, 4}, {}, 5));
    EXPECT_TRUE(Gate("  \t   SwAp  ( 2    \t   ,  4  )             ", 5) == Gate(GateType::SWAP, {2, 4}, {}, 5));
    EXPECT_TRUE(Gate("  \t   SwAp  ( 0x2    \t   ,  4  )             ", 5) == Gate(GateType::SWAP, {2, 4}, {}, 5));

    // CSWAP
    EXPECT_THROW(Gate("CSWAP()", 5), GateException);
    EXPECT_THROW(Gate("CSWAP(2,4;)", 5), GateException);
    EXPECT_THROW(Gate("CSWAP(2,0;4,1)", 5), GateException);
    EXPECT_THROW(Gate("CSWAP(2,0;!!4)", 5), StringException);
    EXPECT_THROW(Gate("CSWAP(2,!0;!4)", 5), StringException);
    EXPECT_THROW(Gate("CSWAP(!2,0;!4)", 5), StringException);
    EXPECT_TRUE(Gate("cswap(2,1;0)", 5) == Gate(GateType::CSWAP, {2, 1}, {{0, true}}, 5));
    EXPECT_TRUE(Gate("  \t   cSWAP(0,1;2)", 5) == Gate(GateType::CSWAP, {0, 1}, {{2, true}}, 5));
    EXPECT_TRUE(Gate("  \t   cSWAP(0,1;!2)", 5) == Gate(GateType::CSWAP, {0, 1}, {{2, false}}, 5));
    EXPECT_TRUE(Gate("  \t   cSWAP(0x0,1;2)", 5) == Gate(GateType::CSWAP, {0, 1}, {{2, true}}, 5));
    EXPECT_TRUE(Gate("  \t   CswaP  ( 2   ,3 \t   ;\t\t  4  )             ", 5) ==
                Gate(GateType::CSWAP, {2, 3}, {{4, true}}, 5));
    EXPECT_TRUE(Gate("  \t   CswaP  ( 2   ,3 \t   ;\t\t  !4  )             ", 5) ==
                Gate(GateType::CSWAP, {2, 3}, {{4, false}}, 5));
}

TEST(Gates, Methods) {
    EXPECT_EQ(Gate("NOT(1)", 4).type(), GateType::NOT);
    EXPECT_EQ(Gate("CNOT(3; 2)", 4).type(), GateType::CNOT);
    EXPECT_EQ(Gate("kCNOT(1; !0, 2, !3)", 4).type(), GateType::kCNOT);
    EXPECT_EQ(Gate("SWAP(2, 1)", 4).type(), GateType::SWAP);
    EXPECT_EQ(Gate("CSWAP(3, 2; 0)", 4).type(), GateType::CSWAP);

    EXPECT_EQ(Gate("NOT(1)", 4).nests(), std::vector<size_t>({1}));
    EXPECT_EQ(Gate("CNOT(3; 2)", 4).nests(), std::vector<size_t>({3}));
    EXPECT_EQ(Gate("kCNOT(1; !0, 2, !3)", 4).nests(), std::vector<size_t>({1}));
    EXPECT_EQ(Gate("SWAP(2, 1)", 4).nests(), std::vector<size_t>({1, 2}));
    EXPECT_EQ(Gate("CSWAP(3, 2; 0)", 4).nests(), std::vector<size_t>({2, 3}));

    EXPECT_EQ(Gate("NOT(1)", 4).controls(), std::vector<size_t>());
    EXPECT_EQ(Gate("CNOT(3; 2)", 4).controls(), std::vector<size_t>({2}));
    EXPECT_EQ(Gate("kCNOT(1; !0, 2, !3)", 4).controls(), std::vector<size_t>({0, 2, 3}));
    EXPECT_EQ(Gate("SWAP(2, 1)", 4).controls(), std::vector<size_t>());
    EXPECT_EQ(Gate("CSWAP(3, 2; 0)", 4).controls(), std::vector<size_t>({0}));

    EXPECT_EQ(Gate("NOT(1)", 4).direct_controls(), std::vector<size_t>());
    EXPECT_EQ(Gate("CNOT(3; 2)", 4).direct_controls(), std::vector<size_t>({2}));
    EXPECT_EQ(Gate("kCNOT(1; !0, 2, !3)", 4).direct_controls(), std::vector<size_t>({2}));
    EXPECT_EQ(Gate("SWAP(2, 1)", 4).direct_controls(), std::vector<size_t>());
    EXPECT_EQ(Gate("CSWAP(3, 2; 0)", 4).direct_controls(), std::vector<size_t>({0}));

    EXPECT_EQ(Gate("NOT(1)", 4).inverted_controls(), std::vector<size_t>());
    EXPECT_EQ(Gate("CNOT(3; 2)", 4).inverted_controls(), std::vector<size_t>());
    EXPECT_EQ(Gate("kCNOT(1; !0, 2, !3)", 4).inverted_controls(), std::vector<size_t>({0, 3}));
    EXPECT_EQ(Gate("SWAP(2, 1)", 4).inverted_controls(), std::vector<size_t>());
    EXPECT_EQ(Gate("CSWAP(3, 2; 0)", 4).inverted_controls(), std::vector<size_t>());
}

TEST(Gates, CommutiesNOT) {
    {
        auto g1 = Gate("NOT(0)", 2);
        auto g2 = Gate("NOT(2)", 3);
        EXPECT_THROW(g1.is_commutes(g2), GateException);
        EXPECT_THROW(g2.is_commutes(g1), GateException);
    }
    {
        auto g1 = Gate("NOT(0)", 3);
        auto g2 = Gate("NOT(0)", 3);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("NOT(0)", 3);
        auto g2 = Gate("NOT(2)", 3);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }

    // NOT and CNOT
    {
        auto g1 = Gate("NOT(0)", 3);
        auto g2 = Gate("CNOT(1; 2)", 3);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("NOT(1)", 3);
        auto g2 = Gate("CNOT(0; 2)", 3);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("NOT(1)", 3);
        auto g2 = Gate("CNOT(1; 2)", 3);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("NOT(1)", 3);
        auto g2 = Gate("CNOT(0; 1)", 3);
        EXPECT_FALSE(g1.is_commutes(g2));
        EXPECT_FALSE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_NE(c1, c2);
    }

    // NOT and kCNOT
    {
        auto g1 = Gate("NOT(0)", 4);
        auto g2 = Gate("kCNOT(1; 2, 3)", 4);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("NOT(2)", 4);
        auto g2 = Gate("kCNOT(1; 0, 3)", 4);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("NOT(2)", 4);
        auto g2 = Gate("kCNOT(2; 0, 3)", 4);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("NOT(0)", 4);
        auto g2 = Gate("kCNOT(1; 0, 3)", 4);
        EXPECT_FALSE(g1.is_commutes(g2));
        EXPECT_FALSE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_NE(c1, c2);
    }
    {
        auto g1 = Gate("NOT(0)", 4);
        auto g2 = Gate("kCNOT(1; !0, 3)", 4);
        EXPECT_FALSE(g1.is_commutes(g2));
        EXPECT_FALSE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_NE(c1, c2);
    }

    // NOT and SWAP
    {
        auto g1 = Gate("NOT(0)", 3);
        auto g2 = Gate("SWAP(1, 2)", 3);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("NOT(1)", 3);
        auto g2 = Gate("SWAP(0, 2)", 3);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("NOT(1)", 3);
        auto g2 = Gate("SWAP(1, 2)", 3);
        EXPECT_FALSE(g1.is_commutes(g2));
        EXPECT_FALSE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_NE(c1, c2);
    }

    // NOT and CSWAP
    {
        auto g1 = Gate("NOT(0)", 4);
        auto g2 = Gate("CSWAP(1, 2; 3)", 4);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("NOT(1)", 4);
        auto g2 = Gate("CSWAP(0, 2; 3)", 4);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("NOT(1)", 4);
        auto g2 = Gate("CSWAP(0, 2; 1)", 4);
        EXPECT_FALSE(g1.is_commutes(g2));
        EXPECT_FALSE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_NE(c1, c2);
    }
    {
        auto g1 = Gate("NOT(1)", 4);
        auto g2 = Gate("CSWAP(0, 2; !1)", 4);
        EXPECT_FALSE(g1.is_commutes(g2));
        EXPECT_FALSE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_NE(c1, c2);
    }
    {
        auto g1 = Gate("NOT(1)", 4);
        auto g2 = Gate("CSWAP(0, 1; 3)", 4);
        EXPECT_FALSE(g1.is_commutes(g2));
        EXPECT_FALSE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_NE(c1, c2);
    }
}

TEST(Gates, CommutiesCNOT) {
    {
        auto g1 = Gate("CNOT(0; 1)", 4);
        auto g2 = Gate("CNOT(2; 3)", 4);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("CNOT(0; 2)", 4);
        auto g2 = Gate("CNOT(1; 3)", 4);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("CNOT(0; 2)", 4);
        auto g2 = Gate("CNOT(1; 2)", 4);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("CNOT(0; 2)", 4);
        auto g2 = Gate("CNOT(2; 3)", 4);
        EXPECT_FALSE(g1.is_commutes(g2));
        EXPECT_FALSE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_NE(c1, c2);
    }

    // CNOT and kCNOT
    {
        auto g1 = Gate("CNOT(0; 1)", 8);
        auto g2 = Gate("kCNOT(2; 3, 4, 5)", 8);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("CNOT(2; 6)", 8);
        auto g2 = Gate("kCNOT(1; 3, 4, 7)", 8);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("CNOT(2; 6)", 8);
        auto g2 = Gate("kCNOT(2; 3, 4, 7)", 8);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("CNOT(2; 6)", 8);
        auto g2 = Gate("kCNOT(1; 2, !6, 7)", 8);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("CNOT(2; !6)", 8);
        auto g2 = Gate("kCNOT(1; 2, 6, 7)", 8);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("CNOT(2; 6)", 8);
        auto g2 = Gate("kCNOT(1; 2, 6, 7)", 8);
        EXPECT_FALSE(g1.is_commutes(g2));
        EXPECT_FALSE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_NE(c1, c2);
    }
    {
        auto g1 = Gate("CNOT(2; 6)", 8);
        auto g2 = Gate("kCNOT(1; 2, 7)", 8);
        EXPECT_FALSE(g1.is_commutes(g2));
        EXPECT_FALSE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_NE(c1, c2);
    }

    // CNOT and SWAP
    {
        auto g1 = Gate("CNOT(0; 1)", 4);
        auto g2 = Gate("SWAP(2, 3)", 4);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("CNOT(0; 2)", 4);
        auto g2 = Gate("SWAP(1, 3)", 4);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("CNOT(0; 1)", 4);
        auto g2 = Gate("SWAP(1, 3)", 4);
        EXPECT_FALSE(g1.is_commutes(g2));
        EXPECT_FALSE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_NE(c1, c2);
    }
    {
        auto g1 = Gate("CNOT(0; 1)", 4);
        auto g2 = Gate("SWAP(0, 3)", 4);
        EXPECT_FALSE(g1.is_commutes(g2));
        EXPECT_FALSE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_NE(c1, c2);
    }

    // CNOT and CSWAP
    {
        auto g1 = Gate("CNOT(0; 1)", 5);
        auto g2 = Gate("CSWAP(2, 3; 4)", 5);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("CNOT(0; 3)", 5);
        auto g2 = Gate("CSWAP(1, 4; 2)", 5);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("CNOT(0; 3)", 5);
        auto g2 = Gate("CSWAP(1, 4; 3)", 5);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("CNOT(0; 1)", 5);
        auto g2 = Gate("CSWAP(0, 3; !1)", 5);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("CNOT(0; !1)", 5);
        auto g2 = Gate("CSWAP(0, 3; 1)", 5);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("CNOT(0; 1)", 5);
        auto g2 = Gate("CSWAP(2, 3; 0)", 5);
        EXPECT_FALSE(g1.is_commutes(g2));
        EXPECT_FALSE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_NE(c1, c2);
    }
    {
        auto g1 = Gate("CNOT(0; 1)", 5);
        auto g2 = Gate("CSWAP(0, 3; 2)", 5);
        EXPECT_FALSE(g1.is_commutes(g2));
        EXPECT_FALSE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_NE(c1, c2);
    }
    {
        auto g1 = Gate("CNOT(0; 1)", 5);
        auto g2 = Gate("CSWAP(1, 3; 4)", 5);
        EXPECT_FALSE(g1.is_commutes(g2));
        EXPECT_FALSE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_NE(c1, c2);
    }
}

TEST(Gates, CommutieskCNOT) {
    {
        auto g1 = Gate("kCNOT(0; 1, 2, 3)", 8);
        auto g2 = Gate("kCNOT(4; 5, 6, 7)", 8);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("kCNOT(0; 1, 5)", 8);
        auto g2 = Gate("kCNOT(4; 2, 7)", 8);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("kCNOT(0; !1, 5)", 8);
        auto g2 = Gate("kCNOT(5; 1, 7)", 8);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("kCNOT(0; 1, 5)", 8);
        auto g2 = Gate("kCNOT(5; 1, 7)", 8);
        EXPECT_FALSE(g1.is_commutes(g2));
        EXPECT_FALSE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_NE(c1, c2);
    }

    // kCNOT and SWAP
    {
        auto g1 = Gate("kCNOT(0; 1, 2, 3)", 8);
        auto g2 = Gate("SWAP(4, 5)", 8);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("kCNOT(3; 0, 6, !7)", 8);
        auto g2 = Gate("SWAP(2, 5)", 8);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("kCNOT(0; 1, 2, !3)", 8);
        auto g2 = Gate("SWAP(1, 2)", 8);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("kCNOT(0; !1, !2, 3)", 8);
        auto g2 = Gate("SWAP(1, 2)", 8);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("kCNOT(0; 1, 2, !3)", 8);
        auto g2 = Gate("SWAP(2, 3)", 8);
        EXPECT_FALSE(g1.is_commutes(g2));
        EXPECT_FALSE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_NE(c1, c2);
    }
    {
        auto g1 = Gate("kCNOT(0; 1, 2, !3)", 8);
        auto g2 = Gate("SWAP(0, 5)", 8);
        EXPECT_FALSE(g1.is_commutes(g2));
        EXPECT_FALSE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_NE(c1, c2);
    }
    {
        auto g1 = Gate("kCNOT(0; 1, 2, !3)", 8);
        auto g2 = Gate("SWAP(1, 5)", 8);
        EXPECT_FALSE(g1.is_commutes(g2));
        EXPECT_FALSE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_NE(c1, c2);
    }
    {
        auto g1 = Gate("kCNOT(0; 1, 2, !3)", 8);
        auto g2 = Gate("SWAP(3, 5)", 8);
        EXPECT_FALSE(g1.is_commutes(g2));
        EXPECT_FALSE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_NE(c1, c2);
    }

    // kCNOT and CSWAP
    {
        auto g1 = Gate("kCNOT(0; 1, 2, 3)", 8);
        auto g2 = Gate("CSWAP(4, 5; 6)", 8);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("kCNOT(5; 0, 4, 7)", 8);
        auto g2 = Gate("CSWAP(1, 6; 2)", 8);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("kCNOT(5; 0, 4, 7)", 8);
        auto g2 = Gate("CSWAP(1, 6; 4)", 8);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("kCNOT(5; 0, 4, 7)", 8);
        auto g2 = Gate("CSWAP(0, 4; 6)", 8);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("kCNOT(5; !0, !4, !7)", 8);
        auto g2 = Gate("CSWAP(0, 4; 6)", 8);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("kCNOT(5; !0, !4, !6)", 8);
        auto g2 = Gate("CSWAP(0, 5; 6)", 8);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("kCNOT(5; !0, !4, 6)", 8);
        auto g2 = Gate("CSWAP(0, 5; !6)", 8);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("kCNOT(5; 0, !4, 7)", 8);
        auto g2 = Gate("CSWAP(0, 4; 6)", 8);
        EXPECT_FALSE(g1.is_commutes(g2));
        EXPECT_FALSE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_NE(c1, c2);
    }
    {
        auto g1 = Gate("kCNOT(6; 0, !4, 7)", 8);
        auto g2 = Gate("CSWAP(0, 3; 6)", 8);
        EXPECT_FALSE(g1.is_commutes(g2));
        EXPECT_FALSE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_NE(c1, c2);
    }
    {
        auto g1 = Gate("kCNOT(3; 0, !4, 7)", 8);
        auto g2 = Gate("CSWAP(0, 3; 6)", 8);
        EXPECT_FALSE(g1.is_commutes(g2));
        EXPECT_FALSE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_NE(c1, c2);
    }
}

TEST(Gates, CommutiesSWAP) {
    {
        auto g1 = Gate("SWAP(0, 1)", 6);
        auto g2 = Gate("SWAP(2, 3)", 6);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("SWAP(0, 3)", 6);
        auto g2 = Gate("SWAP(2, 4)", 6);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("SWAP(0, 3)", 6);
        auto g2 = Gate("SWAP(3, 0)", 6);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("SWAP(0, 3)", 6);
        auto g2 = Gate("SWAP(3, 5)", 6);
        EXPECT_FALSE(g1.is_commutes(g2));
        EXPECT_FALSE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_NE(c1, c2);
    }

    // SWAP and CSWAP
    {
        auto g1 = Gate("SWAP(0, 1)", 6);
        auto g2 = Gate("CSWAP(2, 3; 4)", 6);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("SWAP(3, 5)", 6);
        auto g2 = Gate("CSWAP(0, 4; 2)", 6);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("SWAP(4, 5)", 6);
        auto g2 = Gate("CSWAP(4, 5; 2)", 6);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("SWAP(0, 1)", 6);
        auto g2 = Gate("CSWAP(2, 3; 1)", 6);
        EXPECT_FALSE(g1.is_commutes(g2));
        EXPECT_FALSE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_NE(c1, c2);
    }
    {
        auto g1 = Gate("SWAP(0, 1)", 6);
        auto g2 = Gate("CSWAP(0, 3; 5)", 6);
        EXPECT_FALSE(g1.is_commutes(g2));
        EXPECT_FALSE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_NE(c1, c2);
    }
    {
        auto g1 = Gate("SWAP(1, 3)", 6);
        auto g2 = Gate("CSWAP(2, 3; 1)", 6);
        EXPECT_FALSE(g1.is_commutes(g2));
        EXPECT_FALSE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_NE(c1, c2);
    }
}

TEST(Gates, CommutiesCSWAP) {
    {
        auto g1 = Gate("CSWAP(0, 1; 2)", 8);
        auto g2 = Gate("CSWAP(3, 4; 5)", 8);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("CSWAP(0, 4; 1)", 8);
        auto g2 = Gate("CSWAP(2, 7; 5)", 8);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("CSWAP(0, 4; 1)", 8);
        auto g2 = Gate("CSWAP(4, 0; 5)", 8);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("CSWAP(0, 4; 1)", 8);
        auto g2 = Gate("CSWAP(0, 2; !1)", 8);
        EXPECT_TRUE(g1.is_commutes(g2));
        EXPECT_TRUE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_EQ(c1, c2);
    }
    {
        auto g1 = Gate("CSWAP(0, 4; 1)", 8);
        auto g2 = Gate("CSWAP(0, 2; 1)", 8);
        EXPECT_FALSE(g1.is_commutes(g2));
        EXPECT_FALSE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_NE(c1, c2);
    }
    {
        auto g1 = Gate("CSWAP(0, 4; 1)", 8);
        auto g2 = Gate("CSWAP(0, 2; 6)", 8);
        EXPECT_FALSE(g1.is_commutes(g2));
        EXPECT_FALSE(g2.is_commutes(g1));

        auto c1 = Circuit({g1, g2});
        auto c2 = Circuit({g2, g1});
        EXPECT_NE(c1, c2);
    }
}

TEST(Gates, ActNOT) {
    auto g1 = Gate("NOT(3)", 5);
    auto g2 = Gate("NOT(0)", 5);
    auto g3 = Gate("NOT(1)", 5);
    auto g4 = Gate("NOT(2)", 5);
    auto g5 = Gate("NOT(4)", 5);

    std::vector<bool> vector1({1, 0, 0});
    EXPECT_THROW(g1.act(vector1), GateException);

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
    EXPECT_THROW(g6.act(bf_vector1), GateException);

    std::vector<BooleanFunction> bf_vector2(
            {BooleanFunction("00000000"), BooleanFunction("11010101"), BooleanFunction("1101")});
    EXPECT_THROW(g6.act(bf_vector2), GateException);

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
    auto g3 = Gate("CNOT(3;!2)", 5);
    auto g4 = Gate("CNOT(4;0)", 5);
    auto g5 = Gate("CNOT(1;!3)", 5);

    std::vector<bool> vector1({1, 0, 0});
    EXPECT_THROW(g1.act(vector1), GateException);

    std::vector<bool> vector2({1, 0, 0, 1, 1});
    g1.act(vector2);
    EXPECT_EQ(vector2, (std::vector<bool>{1, 0, 0, 1, 1}));

    g1.act(vector2);
    g2.act(vector2);
    g3.act(vector2);
    g4.act(vector2);
    g5.act(vector2);
    EXPECT_EQ(vector2, (std::vector<bool>{0, 1, 0, 0, 1}));

    // Act for BF
    auto g6 = Gate("CNOT(1; !2)", 3);
    auto g7 = Gate("CNOT(2; !1)", 3);
    auto g8 = Gate("CNOT(0; 1)", 3);

    std::vector<BooleanFunction> bf_vector1(
            {BooleanFunction("00000000"), BooleanFunction("11010101")});
    EXPECT_THROW(g6.act(bf_vector1), GateException);

    std::vector<BooleanFunction> bf_vector2(
            {BooleanFunction("00000000"), BooleanFunction("11010101"), BooleanFunction("1101")});
    EXPECT_THROW(g6.act(bf_vector2), GateException);

    std::vector<BooleanFunction> bf_vector3(
            {BooleanFunction("00000000"), BooleanFunction("11010101"), BooleanFunction("11111110")});
    g6.act(bf_vector3);
    EXPECT_EQ(bf_vector3, (std::vector<BooleanFunction>{BooleanFunction("00000000"), BooleanFunction("11010100"),
                                                        BooleanFunction("11111110")}));

    g6.act(bf_vector3);
    g7.act(bf_vector3);
    g8.act(bf_vector3);
    EXPECT_EQ(bf_vector3, (std::vector<BooleanFunction>{BooleanFunction("11010101"), BooleanFunction("11010101"),
                                                        BooleanFunction("11010100")}));
}

TEST(Gates, ActkCNOT) {
    auto g1 = Gate("kCNOT(2;0,3,4)", 5);
    auto g2 = Gate("kCNOT(0;4)", 5);
    auto g3 = Gate("kCNOT(3;2,1)", 5);
    auto g4 = Gate("kCNOT(4;0,3,1)", 5);
    auto g5 = Gate("kCNOT(1;3,2,4)", 5);

    std::vector<bool> vector1({1, 0, 0});
    EXPECT_THROW(g1.act(vector1), GateException);

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
    EXPECT_THROW(g6.act(bf_vector1), GateException);

    std::vector<BooleanFunction> bf_vector2(
            {BooleanFunction("00000000"), BooleanFunction("11010101"), BooleanFunction("1101")});
    EXPECT_THROW(g6.act(bf_vector2), GateException);

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
    EXPECT_THROW(g1.act(vector1), GateException);

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
    EXPECT_THROW(g6.act(bf_vector1), GateException);

    std::vector<BooleanFunction> bf_vector2(
            {BooleanFunction("00000000"), BooleanFunction("11010101"), BooleanFunction("1101")});
    EXPECT_THROW(g6.act(bf_vector2), GateException);

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
    auto g2 = Gate("CSWAP(0,4;!1)", 5);
    auto g3 = Gate("CSWAP(3,2;4)", 5);
    auto g4 = Gate("CSWAP(4,0;2)", 5);
    auto g5 = Gate("CSWAP(1,2;!0)", 5);

    std::vector<bool> vector1({1, 0, 0});
    EXPECT_THROW(g1.act(vector1), GateException);

    std::vector<bool> vector2({0, 0, 0, 1, 1});
    g1.act(vector2);
    EXPECT_EQ(vector2, (std::vector<bool>{0, 0, 0, 1, 1}));

    g1.act(vector2);
    g2.act(vector2);
    g3.act(vector2);
    g4.act(vector2);
    g5.act(vector2);
    EXPECT_EQ(vector2, (std::vector<bool>{1, 0, 0, 1, 0}));

    // Act for BF
    auto g6 = Gate("CSWAP(1,2;!0)", 3);
    auto g7 = Gate("CSWAP(0,1;2)", 3);
    auto g8 = Gate("CSWAP(0,2;!1)", 3);

    std::vector<BooleanFunction> bf_vector1(
            {BooleanFunction("00000000"), BooleanFunction("11010101")});
    EXPECT_THROW(g6.act(bf_vector1), GateException);

    std::vector<BooleanFunction> bf_vector2(
            {BooleanFunction("00000000"), BooleanFunction("11010101"), BooleanFunction("1101")});
    EXPECT_THROW(g6.act(bf_vector2), GateException);

    std::vector<BooleanFunction> bf_vector3(
            {BooleanFunction("00101010"), BooleanFunction("11010101"), BooleanFunction("11111110")});
    g6.act(bf_vector3);
    EXPECT_EQ(bf_vector3, (std::vector<BooleanFunction>{BooleanFunction("00101010"), BooleanFunction("11010100"),
                                                        BooleanFunction("11111111")}));

    g6.act(bf_vector3);
    g7.act(bf_vector3);
    g8.act(bf_vector3);
    EXPECT_EQ(bf_vector3, (std::vector<BooleanFunction>{BooleanFunction("11010100"), BooleanFunction("00101011"),
                                                        BooleanFunction("11111110")}));
}

TEST(Gates, Stream) {
    std::stringstream out_stream;

    out_stream << Gate(GateType::NOT, {29415}, {}, 100000);
    EXPECT_EQ(out_stream.str(), "NOT(29415)");
    out_stream.str("");

    out_stream << Gate(GateType::kCNOT, {29415}, {{325, true},
                                                  {532, false},
                                                  {1,   false},
                                                  {5,   true}}, 100000);
    EXPECT_EQ(out_stream.str(), "kCNOT(29415; !1, 5, 325, !532)");
    out_stream.str("");

    out_stream << Gate(GateType::CSWAP, {29415, 0}, {{99999, false}}, 100000);
    EXPECT_EQ(out_stream.str(), "CSWAP(0, 29415; !99999)");
    out_stream.str("");
}
