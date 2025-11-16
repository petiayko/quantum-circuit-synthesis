#include <gtest/gtest.h>

#include "gates.hpp"


TEST(Commuties, NOT) {
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

TEST(Commuties, CNOT) {
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

TEST(Commuties, kCNOT) {
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

TEST(Commuties, SWAP) {
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

TEST(Commuties, CSWAP) {
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

TEST(Reduction, ReductionRules) {
    // Rule 1
    {
        Circuit c("Lines: 1\nNOT(0)\nNOT(0)");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(c.schematically_equal(Circuit("Lines: 1")));
    }
    {
        Circuit c("Lines: 4\nkCNOT(2; 0, 1, !3)\nkCNOT(2; 1, 0, !3)\nkCNOT(2; 0, !3, 1)\nkCNOT(2; !3, 1, 0)");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(c.schematically_equal(Circuit("Lines: 4")));
    }
    {
        Circuit c(
                "Lines: 4\nkCNOT(2; 0, 1, !3)\nkCNOT(2; 1, 0, !3)\nkCNOT(2; 0, !3, 1)\nkCNOT(2; !3, 1, 0)\nkCNOT(2; !3, 1, 0)");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(c.schematically_equal(Circuit("Lines: 4\nkCNOT(2; 0, 1, !3)")));
    }
    {
        Circuit c("Lines: 3\nkCNOT(2; 0)\nCNOT(2; 0)");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(c.schematically_equal(Circuit("Lines: 3")));
    }

    // Rule 2
    {
        Circuit c("Lines: 4\nNOT(2)\nkCNOT(3; 0, 2)\nNOT(2)");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(c.schematically_equal(Circuit("Lines: 4\nkCNOT(3; 0, !2)")));
    }
    {
        Circuit c("Lines: 4\nNOT(2)\nkCNOT(3; 0, !2)\nNOT(2)");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(c.schematically_equal(Circuit("Lines: 4\nkCNOT(3; 0, 2)")));
    }
    {
        Circuit c("Lines: 4\nNOT(0)\nNOT(2)\nkCNOT(3; 0, !2)\nNOT(2)\nNOT(0)");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(c.schematically_equal(Circuit("Lines: 4\nkCNOT(3; !0, 2)")));
    }
    {
        Circuit c("Lines: 8\nNOT(0)\nNOT(1)\nNOT(2)\nNOT(3)\nNOT(5)\nNOT(6)\nNOT(7)\n"
                  "kCNOT(4; 0, 1, 2, 3, 5, 6, 7)"
                  "\nNOT(7)\nNOT(6)\nNOT(5)\nNOT(3)\nNOT(2)\nNOT(1)\nNOT(0)\n");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(c.schematically_equal(Circuit("Lines: 8\nkCNOT(4; !0, !1, !2, !3, !5, !6, !7)")));
    }
    {
        Circuit c("Lines: 4\nNOT(0)\nNOT(2)\nkCNOT(3; 0, 2)\nNOT(2)");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(c.schematically_equal(Circuit("Lines: 4\nNOT(0)\nkCNOT(3; 0, !2)")));
    }
    {
        Circuit c("Lines: 4\nNOT(1)\nkCNOT(3; 0, 1)\nNOT(2)");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(c.schematically_equal(Circuit("Lines: 4\nNOT(1)\nkCNOT(3; 0, 1)\nNOT(2)")));
    }
    {
        Circuit c("Lines: 4\nNOT(2)\nkCNOT(3; 0, 1)\nNOT(1)");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(c.schematically_equal(Circuit("Lines: 4\nNOT(2)\nkCNOT(3; 0, 1)\nNOT(1)")));
    }

    // Rule 3
    {
        Circuit c("Lines: 3\nkCNOT(2; !0, 1)\nCNOT(2; !0)");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(c.schematically_equal(Circuit("Lines: 3\nkCNOT(2; !0, !1)")));
    }
    {
        Circuit c("Lines: 3\nCNOT(2; !0)\nkCNOT(2; !0, 1)");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(c.schematically_equal(Circuit("Lines: 3\nkCNOT(2; !0, !1)")));
    }
    {
        Circuit c("Lines: 3\nkCNOT(2; !0, 1)\nCNOT(2; 1)");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(c.schematically_equal(Circuit("Lines: 3\nkCNOT(2; 0, 1)")));
    }
    {
        Circuit c("Lines: 3\nCNOT(2; 1)\nkCNOT(2; !0, 1)");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(c.schematically_equal(Circuit("Lines: 3\nkCNOT(2; 0, 1)")));
    }
    {
        Circuit c("Lines: 2\nCNOT(1; 0)\nNOT(1)");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(c.schematically_equal(Circuit("Lines: 2\nCNOT(1; !0)")));
    }
    {
        Circuit c("Lines: 2\nNOT(1)\nCNOT(1; 0)");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(c.schematically_equal(Circuit("Lines: 2\nCNOT(1; !0)")));
    }

    // Rule 4
    {
        Circuit c("Lines: 3\nkCNOT(2; 0, 1)\nkCNOT(2; 0, !1)");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(c.schematically_equal(Circuit("Lines: 3\nCNOT(2; 0)")));
    }
    {
        Circuit c("Lines: 3\nkCNOT(2; 0, !1)\nkCNOT(2; 0, 1)");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(c.schematically_equal(Circuit("Lines: 3\nCNOT(2; 0)")));
    }
    {
        Circuit c("Lines: 4\nkCNOT(1; 0, 2, 3)\nkCNOT(1; 0, !2, 3)");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(c.schematically_equal(Circuit("Lines: 4\nkCNOT(1; 0, 3)")));
    }
    {
        Circuit c("Lines: 4\nkCNOT(1; 0, !2, 3)\nkCNOT(1; 0, 2, 3)");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(c.schematically_equal(Circuit("Lines: 4\nkCNOT(1; 0, 3)")));
    }
    {
        Circuit c("Lines: 3\nkCNOT(1; 0, 2)\nkCNOT(1; !0, !2)");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(c.schematically_equal(Circuit("Lines: 3\nkCNOT(1; 0, 2)\nkCNOT(1; !0, !2)")));
    }
    {
        Circuit c("Lines: 3\nkCNOT(1; !0, !2)\nkCNOT(1; 0, 2)");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(c.schematically_equal(Circuit("Lines: 3\nkCNOT(1; !0, !2)\nkCNOT(1; 0, 2)")));
    }

    // Rule 5
    {
        Circuit c("Lines: 3\nkCNOT(2; 0, !1)\nkCNOT(2; !0, 1)");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(c.schematically_equal(Circuit("Lines: 3\nCNOT(2; 0)\nCNOT(2; 1)")));
    }
    {
        Circuit c("Lines: 3\nkCNOT(2; !0, 1)\nkCNOT(2; 0, !1)");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(c.schematically_equal(Circuit("Lines: 3\nCNOT(2; 1)\nCNOT(2; 0)")));
    }
    {
        Circuit c("Lines: 4\nkCNOT(3; 0, !1, 2)\nkCNOT(3; 0, 1, !2)");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(c.schematically_equal(Circuit("Lines: 4\nkCNOT(3; 0, 2)\nkCNOT(3; 0, 1)")));
    }
    {
        Circuit c("Lines: 4\nkCNOT(3; 0, 1, !2)\nkCNOT(3; 0, !1, 2)");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(c.schematically_equal(Circuit("Lines: 4\nkCNOT(3; 0, 1)\nkCNOT(3; 0, 2)")));
    }
    {
        Circuit c("Lines: 8\nkCNOT(7; !0, 1, 2, 3, !4, 5, !6)\nkCNOT(7; 0, 1, !2, 3, !4, 5, !6)");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(
                c.schematically_equal(Circuit("Lines: 8\nkCNOT(7; 1, 2, 3, !4, 5, !6)\nkCNOT(7; 0, 1, 3, !4, 5, !6)")));
    }
    {
        Circuit c("Lines: 8\nkCNOT(7; 0, 1, !2, 3, !4, 5, !6)\nkCNOT(7; !0, 1, 2, 3, !4, 5, !6)");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(
                c.schematically_equal(Circuit("Lines: 8\nkCNOT(7; 0, 1, 3, !4, 5, !6)\nkCNOT(7; 1, 2, 3, !4, 5, !6)")));
    }
    {
        Circuit c("Lines: 8\nkCNOT(7; !0, !1, 2, !3, !4, !5, !6)\nkCNOT(7; 0, !1, !2, !3, !4, !5, !6)");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(c.schematically_equal(
                Circuit("Lines: 8\nkCNOT(7; !1, 2, !3, !4, !5, !6)\nkCNOT(7; 0, !1, !3, !4, !5, !6)")));
    }
    {
        Circuit c("Lines: 8\nkCNOT(7; 0, !1, !2, !3, !4, !5, !6)\nkCNOT(7; !0, !1, 2, !3, !4, !5, !6)");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(c.schematically_equal(
                Circuit("Lines: 8\nkCNOT(7; 0, !1, !3, !4, !5, !6)\nkCNOT(7; !1, 2, !3, !4, !5, !6)")));
    }
    {
        Circuit c("Lines: 8\nkCNOT(7; !0, !1, 2, !3, !4, !5, !6)\nkCNOT(7; 0, !1, !2, !3, !4, !5, !6)");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(c.schematically_equal(
                Circuit("Lines: 8\nkCNOT(7; !1, 2, !3, !4, !5, !6)\nkCNOT(7; 0, !1, !3, !4, !5, !6)")));
    }
    {
        Circuit c("Lines: 8\nkCNOT(7; 0, 1, !2, !3, !4, !5, !6)\nkCNOT(7; !0, !1, 2, 3, !4, !5, !6)");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(c.schematically_equal(
                Circuit("Lines: 8\nkCNOT(7; 0, 1, !2, !3, !4, !5, !6)\nkCNOT(7; !0, !1, 2, 3, !4, !5, !6)")));
    }
    {
        Circuit c("Lines: 8\nkCNOT(7; !0, !1, 2, 3, !4, !5, !6)\nkCNOT(7; 0, 1, !2, !3, !4, !5, !6)");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(c.schematically_equal(
                Circuit("Lines: 8\nkCNOT(7; !0, !1, 2, 3, !4, !5, !6)\nkCNOT(7; 0, 1, !2, !3, !4, !5, !6)")));
    }

    // Rule 6
    {
        Circuit c("Lines: 4\nkCNOT(3; !1, 2)\nkCNOT(3; !0, 2)\nkCNOT(0; !1, 2)");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(c.schematically_equal(Circuit("Lines: 4\nkCNOT(0; !1, 2)\nkCNOT(3; !0, 2)")));
    }
    {
        Circuit c("Lines: 4\nkCNOT(0; 1, !2)\nkCNOT(3; 0, !2)\nkCNOT(3; 1, !2)");
        Circuit c_copy(c);
        c.reduce();
        EXPECT_EQ(c, c_copy);
        EXPECT_TRUE(c.schematically_equal(Circuit("Lines: 4\nkCNOT(3; 0, !2)\nkCNOT(0; 1, !2)")));
    }
}

TEST(Reduction, MoveSwapLeft) {
    {
        Circuit c("Lines: 3");
        Circuit c1(c);
        EXPECT_EQ(c.move_swap_left(), 0);
        EXPECT_TRUE(c.schematically_equal(c1));
    }
    {
        Circuit c("Lines: 3\nNOT(2)\nCNOT(2; 1)\nCSWAP(2, 0; 1)");
        Circuit c1(c);
        EXPECT_EQ(c.move_swap_left(), 0);
        EXPECT_TRUE(c.schematically_equal(c1));
    }
    {
        Circuit c("Lines: 3\nSWAP(0, 1)\nSWAP(2, 1)\nkCNOT(2; 0, 1)");
        Circuit c1(c);
        EXPECT_EQ(c.move_swap_left(), 2);
        EXPECT_TRUE(c.schematically_equal(c1));
    }
    {
        Circuit c("Lines: 3\nSWAP(0, 1)\nSWAP(2, 1)\nSWAP(0, 2)");
        Circuit c1(c);
        EXPECT_EQ(c.move_swap_left(), 3);
        EXPECT_TRUE(c.schematically_equal(c1));
    }
    {
        Circuit c("Lines: 3\nSWAP(0, 1)\nCNOT(2; 1)\nSWAP(0, 2)");
        Circuit c1(c);
        EXPECT_EQ(c.move_swap_left(), 2);
        EXPECT_FALSE(c.schematically_equal(c1));
        EXPECT_EQ(c, c1);
    }
    {
        Circuit c("Lines: 3\nNOT(2)\nSWAP(0, 2)\nSWAP(0, 1)\nSWAP(1, 2)\nSWAP(0, 2)\nSWAP(0, 1)\nSWAP(1, 2)\nNOT(0)");
        Circuit c1(c);
        EXPECT_EQ(c.move_swap_left(), 6);
        EXPECT_FALSE(c.schematically_equal(c1));
        EXPECT_EQ(c, c1);
    }
    {
        Circuit c("Lines: 4\nSWAP(3, 1)\nCNOT(0; 1)\nCSWAP(3, 1; 2)\nkCNOT(2; 0, 1, 3)\nSWAP(1, 2)");
        Circuit c1(c);
        EXPECT_EQ(c.move_swap_left(), 2);
        EXPECT_FALSE(c.schematically_equal(c1));
        EXPECT_EQ(c, c1);
    }
    {
        Circuit c("Lines: 4\nNOT(2)\nNOT(0)\nSWAP(3, 1)\nCNOT(0; !1)\nCSWAP(3, 1; 0)\nkCNOT(2; 0, !1, 3)\nSWAP(1, 0)");
        Circuit c1(c);
        EXPECT_EQ(c.move_swap_left(), 2);
        EXPECT_FALSE(c.schematically_equal(c1));
        EXPECT_EQ(c, c1);
    }
}

TEST(Reduction, Reduction) {
    {
        Circuit c("Lines: 4");
        Circuit c_copy(c);
        c.reduce();

        EXPECT_EQ(c, c_copy);
        EXPECT_EQ(c.complexity(), c_copy.complexity());
        EXPECT_TRUE(c.schematically_equal(c_copy));
    }
    {
        Circuit c("Lines: 4\nkCNOT(3; !1, 2)");
        Circuit c_copy(c);
        c.reduce();

        EXPECT_EQ(c, c_copy);
        EXPECT_EQ(c.complexity(), c_copy.complexity());
        EXPECT_TRUE(c.schematically_equal(c_copy));
    }
    {
        Circuit c("Lines: 4\nSWAP(3, 1)\nSWAP(0, 2)\nSWAP(0, 3)");
        Circuit c_copy(c);
        c.reduce();

        EXPECT_EQ(c, c_copy);
        EXPECT_EQ(c.complexity(), c_copy.complexity());
        EXPECT_TRUE(c.schematically_equal(c_copy));
    }
    {
        Circuit c("Lines: 4\nkCNOT(3; !1, 2)\nkCNOT(3; !1, 2)");
        Circuit c_copy(c);
        c.reduce();

        EXPECT_EQ(c, c_copy);
        EXPECT_LT(c.complexity(), c_copy.complexity());
        EXPECT_FALSE(c.schematically_equal(c_copy));
    }
    {
        Circuit c("Lines: 4\nkCNOT(3; !1, 2)\nNOT(0)\nkCNOT(3; !1, 2)");
        Circuit c_copy(c);
        c.reduce();

        EXPECT_EQ(c, c_copy);
        EXPECT_LT(c.complexity(), c_copy.complexity());
        EXPECT_FALSE(c.schematically_equal(c_copy));
    }
    {
        Circuit c("Lines: 4\nkCNOT(3; !1, 2)\nNOT(0)\nkCNOT(3; !1, 2)\nSWAP(2, 3)");
        Circuit c_copy(c);
        c.reduce();

        EXPECT_EQ(c, c_copy);
        EXPECT_LT(c.complexity(), c_copy.complexity());
        EXPECT_FALSE(c.schematically_equal(c_copy));
    }
    {
        Circuit c("Lines: 4\nkCNOT(2; !1, 3)\nNOT(0)\nkCNOT(2; !1)");
        Circuit c_copy(c);
        c.reduce();

        EXPECT_EQ(c, c_copy);
        EXPECT_LT(c.complexity(), c_copy.complexity());
        EXPECT_FALSE(c.schematically_equal(c_copy));
    }
    {
        Circuit c("Lines: 4\nkCNOT(2; !0, 3)\nNOT(1)\nSWAP(0, 1)\nkCNOT(2; !1)");
        Circuit c_copy(c);
        c.reduce();

        EXPECT_EQ(c, c_copy);
        EXPECT_LT(c.complexity(), c_copy.complexity());
        EXPECT_FALSE(c.schematically_equal(c_copy));
    }
    {
        Circuit c("Lines: 4\nNOT(1)\nkCNOT(2; 0, 1, 3)\nkCNOT(2; !0, 3, 1)");
        Circuit c_copy(c);
        c.reduce();

        EXPECT_EQ(c, c_copy);
        EXPECT_LT(c.complexity(), c_copy.complexity());
        EXPECT_FALSE(c.schematically_equal(c_copy));
    }
    {
        Circuit c("Lines: 4\nNOT(1)\nkCNOT(2; 0, 1, 3)\nCSWAP(0, 2; 3)\nCNOT(2; 1)\nNOT(2)");
        Circuit c_copy(c);
        c.reduce();

        EXPECT_EQ(c, c_copy);
        EXPECT_LT(c.complexity(), c_copy.complexity());
        EXPECT_FALSE(c.schematically_equal(c_copy));
    }
    {
        Circuit c("Lines: 4\nNOT(1)\nkCNOT(2; 0, 1, 3)\nCSWAP(0, 2; 3)\nCNOT(2; 1)\nCNOT(1; 2)");
        Circuit c_copy(c);
        c.reduce();

        EXPECT_EQ(c, c_copy);
        EXPECT_EQ(c.complexity(), c_copy.complexity());
        EXPECT_TRUE(c.schematically_equal(c_copy));
    }

    {
        Circuit c("Lines: 4\nSWAP(0, 1)\nkCNOT(2; 0, !1)\nCNOT(3; 1)\nkCNOT(2; !0, 1)\nCNOT(2; 1)\nCNOT(3; 1)\n"
                  "SWAP(2, 3)\nCNOT(2; 1)\nkCNOT(2; !0, 1)\nCNOT(0; 1)");
        Circuit c_copy(c);
        c.reduce();

        EXPECT_EQ(c, c_copy);
        EXPECT_EQ(c.complexity(), 5);  // см отчет
        EXPECT_FALSE(c.schematically_equal(c_copy));
    }
}
