#include <gtest/gtest.h>

#include "gates.hpp"

TEST(Circuits, Constructor) {
    EXPECT_THROW(Circuit(""), std::runtime_error);
    EXPECT_THROW(Circuit("Lines: -1"), std::runtime_error);
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

TEST(Circuits, Act) {
    std::vector<bool> vec{0, 1};
    EXPECT_THROW(Circuit("Lines: 0").act(vec), std::runtime_error);
    EXPECT_THROW(Circuit("Lines: 1").act(vec), std::runtime_error);
    EXPECT_THROW(Circuit("Lines: 3").act(vec), std::runtime_error);

    Circuit c("Lines: 4");
    std::vector<bool> vec1{0, 1, 1, 1};
    c.act(vec1);
    EXPECT_EQ(vec1, (std::vector<bool>{0, 1, 1, 1}));

    c.add(Gate("NOT(2)", 4));
    c.add(Gate("CSWAP(1,2;3)", 4));
    c.add(Gate("SWAP(0,1)", 4));
    c.add(Gate("SWAP(1,2)", 4));
    c.act(vec1);
    EXPECT_EQ(vec1, (std::vector<bool>{0, 1, 0, 1}));

    Circuit c1("Lines: 2");
    std::vector<BooleanFunction> vec_bf{BooleanFunction("0110")};
    EXPECT_THROW(c1.act(vec_bf), std::runtime_error);
    std::vector<BooleanFunction> vec_bf1{BooleanFunction("0110"), BooleanFunction("1101"), BooleanFunction("0000")};
    EXPECT_THROW(c1.act(vec_bf1), std::runtime_error);
    std::vector<BooleanFunction> vec_bf2{BooleanFunction("0110"), BooleanFunction("11010010")};
    EXPECT_THROW(c1.act(vec_bf2), std::runtime_error);

    std::vector<BooleanFunction> vec_bf3{BooleanFunction("0110"), BooleanFunction("0100")};
    c1.act(vec_bf3);
    EXPECT_EQ(vec_bf3, (std::vector<BooleanFunction>{BooleanFunction("0110"), BooleanFunction("0100")}));

    c1.add(Gate("NOT(0)", 2));
    c1.add(Gate("SWAP(0,1)", 2));
    c1.add(Gate("kCNOT(1;0)", 2));
    c1.act(vec_bf3);
    EXPECT_EQ(vec_bf3, (std::vector<BooleanFunction>{BooleanFunction("0100"), BooleanFunction("1101")}));
}

TEST(Circuits, Stream) {
    std::stringstream out_stream;

    out_stream << Circuit(5);
    EXPECT_EQ(out_stream.str(), "Lines: 5\n");
    out_stream.str("");

    Circuit c(4);
    for (size_t _ = 0; _ < 10; _++) {
        c.add(Gate("NOT(3)", 4));
    }
    for (size_t _ = 0; _ < 6; _++) {
        c.add(Gate("CNOT(2;1)", 4));
    }
    for (size_t _ = 0; _ < 3; _++) {
        c.add(Gate("CSWAP(3,0;1)", 4));
    }
    for (size_t _ = 0; _ < 4; _++) {
        c.add(Gate("kCNOT(2;0,1,3)", 4));
    }
    out_stream << c;
    EXPECT_EQ(out_stream.str(), "Lines: 4\n"
                                "NOT(3)\n"
                                "NOT(3)\n"
                                "NOT(3)\n"
                                "NOT(3)\n"
                                "NOT(3)\n"
                                "NOT(3)\n"
                                "NOT(3)\n"
                                "NOT(3)\n"
                                "NOT(3)\n"
                                "NOT(3)\n"
                                "CNOT(2; 1)\n"
                                "CNOT(2; 1)\n"
                                "CNOT(2; 1)\n"
                                "CNOT(2; 1)\n"
                                "CNOT(2; 1)\n"
                                "CNOT(2; 1)\n"
                                "CSWAP(3, 0; 1)\n"
                                "CSWAP(3, 0; 1)\n"
                                "CSWAP(3, 0; 1)\n"
                                "kCNOT(2; 0, 1, 3)\n"
                                "kCNOT(2; 0, 1, 3)\n"
                                "kCNOT(2; 0, 1, 3)\n"
                                "kCNOT(2; 0, 1, 3)\n");

    std::ifstream file("../tests/assets/qc.txt", std::ios::in);
    Circuit c1(file);
    EXPECT_EQ(c1, Circuit(std::vector<Gate>{Gate("NOT(1)", 4), Gate("CSWAP(3,1;0)", 4), Gate("kCNOT(0;1,2,3)", 4)}));
}
