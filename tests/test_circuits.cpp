#include <gtest/gtest.h>

#include "gates.hpp"


TEST(Circuits, Constructor) {
    EXPECT_THROW(Circuit(""), CircuitException);
    EXPECT_THROW(Circuit("Lines: -1"), CircuitException);
    EXPECT_THROW(Circuit("Lines: 0"), CircuitException);
    EXPECT_THROW(Circuit("#Lines: 2"), CircuitException);
    EXPECT_THROW(Circuit("Lines 3\nNOT(1)"), CircuitException);
    EXPECT_THROW(Circuit("4\nNOT(1)"), CircuitException);
    EXPECT_THROW(Circuit(": 4\nNOT(1)"), CircuitException);
    EXPECT_THROW(Circuit("lines: 2\nNOT(3)"), GateException);
    EXPECT_THROW(Circuit("NOT(3)\nCNOT(0;1)\n"), CircuitException);
    EXPECT_THROW(Circuit("NOT(3)\nCNOT(0;1)\nlines: 2"), CircuitException);
    EXPECT_THROW(Circuit("Lines: 3\n; # 1"), GateException);
    EXPECT_THROW(Circuit("Lines: 3\n,1"), GateException);
    EXPECT_THROW(Circuit("Lines: 13, 2"), CircuitException);
    EXPECT_THROW(Circuit("Lines: 13 4"), CircuitException);
    EXPECT_THROW(Circuit("Lines: 13; 100"), CircuitException);

    EXPECT_EQ(Circuit("lines: 3;0"), Circuit(3));
    EXPECT_EQ(Circuit("lines: 3;2"), Circuit(3, 2));
    EXPECT_EQ(Circuit("lINES: 3;1\n"), Circuit(3, 1));
    EXPECT_EQ(Circuit("#\nLines: 3\n"), Circuit(3));
    EXPECT_EQ(Circuit("\n\n\n#\n\n\n\nLines: 3\n\n\n\n"), Circuit(3));
    EXPECT_EQ(Circuit("\t\t     \tLines: \t\t\t       A\t\t\t\t"), Circuit(10));
    EXPECT_EQ(Circuit("\t\t     \tLines: \t\t\t       A\t\t\t\t ;  \t0x6"), Circuit(10, 6));
    EXPECT_EQ(Circuit("Lines: A; 0x7\n"), Circuit(10, 7));

    EXPECT_THROW(Circuit(2, 10), CircuitException);

    EXPECT_EQ(Circuit(0, 0), Circuit());
    EXPECT_EQ(Circuit(0, 0), Circuit(0));

    EXPECT_THROW(Circuit("Lines: A\n# text\n#comment\nNOT(B)\n#CNOT(3;0xc)\n\n\nSWAP(8,9)"), GateException);
    EXPECT_THROW(Circuit("Lines: A;2\n# text\n#comment\nNOT(B)\n#CNOT(3;0xc)\n\n\nSWAP(8,9)"), GateException);
    EXPECT_THROW(Circuit(std::vector<Gate>{Gate("NOT(2)", 10), Gate("SWAP(8,9)", 10)}, 10), CircuitException);

    EXPECT_EQ(Circuit("Lines: A\n# text\n#comment\nNOT(2)\n#CNOT(3;9)\n\n\nSWAP(8,9)"),
              Circuit(std::vector<Gate>{Gate("NOT(2)", 10), Gate("SWAP(8,9)", 10)}));
    EXPECT_EQ(Circuit(std::vector<Gate>{Gate("NOT(2)", 10), Gate("SWAP(8,9)", 10)}, 8),
              Circuit("Lines: 10;8\nNOT(2)\nSWAP(8,9)"));
}

TEST(Circuits, Act) {
    std::vector<bool> vec{0, 1};
    EXPECT_THROW(Circuit("Lines: 0").act(vec), CircuitException);
    EXPECT_THROW(Circuit("Lines: 1").act(vec), CircuitException);
    EXPECT_THROW(Circuit("Lines: 3").act(vec), CircuitException);

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
    EXPECT_THROW(c1.act(vec_bf), CircuitException);
    std::vector<BooleanFunction> vec_bf1{BooleanFunction("0110"), BooleanFunction("1101"), BooleanFunction("0000")};
    EXPECT_THROW(c1.act(vec_bf1), CircuitException);
    std::vector<BooleanFunction> vec_bf2{BooleanFunction("0110"), BooleanFunction("11010010")};
    EXPECT_THROW(c1.act(vec_bf2), CircuitException);

    std::vector<BooleanFunction> vec_bf3{BooleanFunction("0110"), BooleanFunction("0100")};
    c1.act(vec_bf3);
    EXPECT_EQ(vec_bf3, (std::vector<BooleanFunction>{BooleanFunction("0110"), BooleanFunction("0100")}));

    c1.add(Gate("NOT(0)", 2));
    c1.add(Gate("SWAP(0,1)", 2));
    c1.add(Gate("kCNOT(1;0)", 2));
    c1.act(vec_bf3);
    EXPECT_EQ(vec_bf3, (std::vector<BooleanFunction>{BooleanFunction("0100"), BooleanFunction("1101")}));
}

TEST(Circuits, Memory) {
    Circuit c("Lines: 4");
    EXPECT_EQ(c.memory(), 0);
    EXPECT_EQ(c.dim(), 4);

    c.set_memory(2);
    EXPECT_EQ(c.memory(), 2);
    EXPECT_EQ(c.dim(), 4);

    c.set_memory(3);
    EXPECT_EQ(c.memory(), 3);
    EXPECT_EQ(c.dim(), 4);

    EXPECT_THROW(c.set_memory(5), CircuitException);

    c.set_memory(0);
    EXPECT_EQ(c.memory(), 0);
    EXPECT_EQ(c.dim(), 4);

    Circuit c1("Lines: 4; 0");
    std::vector<bool> vec{0, 1, 1, 1};
    c.act(vec);
    EXPECT_EQ(vec, (std::vector<bool>{0, 1, 1, 1}));

    c1.set_memory(1);
    c1.act(vec);
    EXPECT_EQ(vec, (std::vector<bool>{0, 1, 1, 0}));

    c1.set_memory(3);
    vec = {0, 1, 1, 1};
    c1.act(vec);
    EXPECT_EQ(vec, (std::vector<bool>{0, 0, 0, 0}));

    Circuit c2("Lines: 3; 1");
    std::vector<BooleanFunction> vec_bf{BooleanFunction("01101000"), BooleanFunction("11110111"),
                                        BooleanFunction("01000100")};
    c2.act(vec_bf);
    EXPECT_EQ(vec_bf, (std::vector<BooleanFunction>{BooleanFunction("0110"), BooleanFunction("1101"),
                                                    BooleanFunction("0000")}));

    c2.add(Gate("NOT(0)", 3));
    c2.add(Gate("SWAP(0,1)", 3));
    c2.add(Gate("kCNOT(2;0)", 3));
    vec_bf = {BooleanFunction("01101000"), BooleanFunction("11110111"), BooleanFunction("01000100")};
    c2.act(vec_bf);
    EXPECT_EQ(vec_bf, (std::vector<BooleanFunction>{BooleanFunction("1101"), BooleanFunction("1001"),
                                                    BooleanFunction("1101")}));

    c2.set_memory(2);
    vec_bf = {BooleanFunction("01101000"), BooleanFunction("11110111"), BooleanFunction("01000100")};
    c2.act(vec_bf);
    EXPECT_EQ(vec_bf,
              (std::vector<BooleanFunction>{BooleanFunction("10"), BooleanFunction("10"), BooleanFunction("10")}));
}

TEST(Circuits, Production) {
    {
        Circuit c("Lines: 4");
        EXPECT_EQ(c.produce_mapping(), Substitution("0 1 2 3 4 5 6 7 8 9 A B C D E F"));
    }
    {
        Circuit c("Lines: 3\nNOT(2)\nCNOT(2; 1)\nCNOT(2; 0)\nkCNOT(2; 0, 1)");
        EXPECT_EQ(c.produce_mapping(), Substitution("1 0 2 3 4 5 6 7"));
    }
    {
        Circuit c("Lines: 3\n"
                  "NOT(2)\nCNOT(2; 1)\nCNOT(2; 0)\nkCNOT(2; 0, 1)\n"
                  "kCNOT(2; 0, 1)\nCNOT(2; 0)\nCNOT(2; 1)\nNOT(2)");
        EXPECT_EQ(c.produce_mapping(), Substitution("0 1 2 3 4 5 6 7"));
    }
    {
        Circuit c("Lines: 3; 1\nNOT(2)\nCNOT(2; 1)\nCNOT(2; 0)\nkCNOT(2; 0, 1)");
        EXPECT_EQ(c.produce_mapping(), BinaryMapping(table{{0, 0, 1, 1},
                                                           {0, 1, 0, 1},
                                                           {1, 0, 0, 0}}));
    }
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

    std::ifstream file1("../tests/assets/map.txt", std::ios::in);
    EXPECT_THROW((Circuit(file1)), CircuitException);

    std::ifstream file2("../tests/assets/sub.txt", std::ios::in);
    EXPECT_THROW((Circuit(file2)), CircuitException);
}
