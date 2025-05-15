#include <gtest/gtest.h>

#include "gates.hpp"

TEST(Circuits, Constructor) {
    EXPECT_THROW(Circuit(""), std::runtime_error);
    EXPECT_THROW(Circuit("Lines: 0"), std::runtime_error);
    EXPECT_THROW(Circuit("#Lines: 2"), std::runtime_error);
    EXPECT_THROW(Circuit("Lines 3\nNOT(1)"), std::runtime_error);
    EXPECT_THROW(Circuit("4\nNOT(1)"), std::runtime_error);
    EXPECT_THROW(Circuit(": 4\nNOT(1)"), std::runtime_error);
    EXPECT_THROW(Circuit("lines: 2\nNOT(3)"), std::runtime_error);
    EXPECT_THROW(Circuit("NOT(3)\nCNOT(0;1)\n"), std::runtime_error);
    EXPECT_THROW(Circuit("NOT(3)\nCNOT(0;1)\nlines: 2"), std::runtime_error);

    EXPECT_EQ(Circuit("lines: 3"), Circuit(3));
    EXPECT_EQ(Circuit("lINES: 3\n"), Circuit(3));
    EXPECT_EQ(Circuit("#\nLines: 3\n"), Circuit(3));
    EXPECT_EQ(Circuit("\n\n\n#\n\n\n\nLines: 3\n\n\n\n"), Circuit(3));
    EXPECT_EQ(Circuit("\t\t     \tLines: \t\t\t       A\t\t\t\t"), Circuit(10));

    EXPECT_EQ(Circuit("Lines: A\n# text\n#comment\nNOT(2)\n#CNOT(3;9)\n\n\nSWAP(8,9)"),
              Circuit(std::vector<Gate>{Gate("NOT(2)", 10), Gate("SWAP(8,9)", 10)}));
}

TEST(Circuits, Stream) {
    std::ifstream file("../tests/assets/circuit.txt", std::ios::in);
    Circuit c(file);
    EXPECT_EQ(c, Circuit(std::vector<Gate>{Gate("NOT(1)", 4), Gate("CSWAP(3,1;0)", 4), Gate("kCNOT(0;1,2,3)", 4)}));
}
