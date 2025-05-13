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
    EXPECT_NO_THROW(Gate(kCNOT, {0}, {1, 3, 2, 4}, 5));

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
    EXPECT_TRUE(Gate("kcnot(2;4, 1)", 5) == Gate(kCNOT, {2}, {1, 4}, 5));
    EXPECT_TRUE(Gate("  \t   Kcnot(2;1,0)", 5) == Gate(kCNOT, {2}, {0, 1}, 5));
    EXPECT_TRUE(Gate("  \t   kCnOt  ( 2    ;\t3   ,  4  )             ", 5) == Gate(kCNOT, {2}, {3, 4}, 5));

    // SWAP
    EXPECT_THROW(Gate("SWAP()", 5), std::runtime_error);
    EXPECT_THROW(Gate("SWAP(2,4;)", 5), std::runtime_error);
    EXPECT_THROW(Gate("SWAP(2;4)", 5), std::runtime_error);
    EXPECT_TRUE(Gate("swap(2,1)", 5) == Gate(SWAP, {1, 2}, {}, 5));
    EXPECT_TRUE(Gate("  \t   SWap(2,4)", 5) == Gate(SWAP, {2, 4}, {}, 5));
    EXPECT_TRUE(Gate("  \t   SwAp  ( 2    \t   ,  4  )             ", 5) == Gate(SWAP, {2, 4}, {}, 5));

    // CSWAP
    EXPECT_THROW(Gate("CSWAP()", 5), std::runtime_error);
    EXPECT_THROW(Gate("CSWAP(2,4;)", 5), std::runtime_error);
    EXPECT_THROW(Gate("CSWAP(2,0;4,1)", 5), std::runtime_error);
    EXPECT_TRUE(Gate("cswap(2,1;0)", 5) == Gate(CSWAP, {1, 2}, {0}, 5));
    EXPECT_TRUE(Gate("  \t   cSWAP(0,1;2)", 5) == Gate(CSWAP, {0, 1}, {2}, 5));
    EXPECT_TRUE(Gate("  \t   CswaP  ( 2   ,3 \t   ;\t\t  4  )             ", 5) == Gate(CSWAP, {2, 3}, {4}, 5));
}
