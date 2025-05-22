#include <gtest/gtest.h>

#include "synthesis.hpp"


TEST(Synthesis, General) {
    Substitution sub("0 1");
    EXPECT_THROW(synthesize(sub, ""), SynthException);
    EXPECT_THROW(synthesize(sub, "algo"), SynthException);
    EXPECT_THROW(synthesize(sub, "enum"), SynthException);
    EXPECT_THROW(synthesize(sub, "RW"), SynthException);
    EXPECT_THROW(synthesize(sub, "Dummy"), SynthException);
    EXPECT_NO_THROW(synthesize(sub));

    BinaryMapping bm("0 1\n1 0");
    EXPECT_THROW(synthesize(bm, ""), SynthException);
    EXPECT_THROW(synthesize(bm, "algo"), SynthException);
    EXPECT_THROW(synthesize(bm, "enum"), SynthException);
    EXPECT_THROW(synthesize(bm, "RW"), SynthException);
    EXPECT_THROW(synthesize(bm, "Dummy"), SynthException);
    EXPECT_NO_THROW(synthesize(bm));
}

TEST(Synthesis, MappingDummy) {
    {
        BinaryMapping bm(table{{0, 0}});
        Circuit c("Lines: 2; 1");
        EXPECT_EQ(synthesize(bm, "dummy"), c);
        EXPECT_EQ(dummy_algorithm(bm), c);
    }
    {
        BinaryMapping bm(table{{1, 1}});
        Circuit c("Lines: 2; 1\nNOT(1)");
        EXPECT_EQ(synthesize(bm, "dummy"), c);
        EXPECT_EQ(dummy_algorithm(bm), c);
    }
    {
        BinaryMapping bm(table{{0, 0, 0, 0, 0, 0, 0, 0}});
        Circuit c("Lines: 4; 1");
        EXPECT_EQ(synthesize(bm, "dummy"), c);
        EXPECT_EQ(dummy_algorithm(bm), c);
    }
    {
        BinaryMapping bm(table{{1, 1, 1, 1, 1, 1, 1, 1}});
        Circuit c("Lines: 4; 1\nNOT(3)");
        EXPECT_EQ(synthesize(bm, "dummy"), c);
        EXPECT_EQ(dummy_algorithm(bm), c);
    }
    {
        BinaryMapping bm(table{{1, 0, 0, 0}});
        Circuit c("Lines: 3; 1\nNOT(2)\nkCNOT(2; 1)\nkCNOT(2; 0)\nkCNOT(2; 0, 1)");
        EXPECT_EQ(synthesize(bm, "dummy"), c);
        EXPECT_EQ(dummy_algorithm(bm), c);
    }
    {
        BinaryMapping bm(table{{1, 0, 0, 0},
                               {0, 0, 1, 0}});
        Circuit c("Lines: 4; 2\n"
                  "NOT(2)\nkCNOT(2; 1)\nkCNOT(2; 0)\nkCNOT(2; 0, 1)\n"
                  "kCNOT(3; 0)\nkCNOT(3; 0, 1)");
        EXPECT_EQ(synthesize(bm, "dummy"), c);
        EXPECT_EQ(dummy_algorithm(bm), c);
    }
    {
        BinaryMapping bm(table{{1, 0, 0, 0},
                               {0, 0, 1, 0},
                               {0, 0, 1, 0},
                               {1, 1, 1, 0},
                               {1, 0, 1, 0}});
        Circuit c("Lines: 7; 5\n"
                  "NOT(2)\nkCNOT(2; 1)\nkCNOT(2; 0)\nkCNOT(2; 0, 1)\n"
                  "kCNOT(3; 0)\nkCNOT(3; 0, 1)\n"
                  "kCNOT(4; 0)\nkCNOT(4; 0, 1)\n"
                  "NOT(5)\nkCNOT(5; 0, 1)\n"
                  "NOT(6)\nkCNOT(6; 1)");
        EXPECT_EQ(synthesize(bm, "dummy"), c);
        EXPECT_EQ(dummy_algorithm(bm), c);
    }
}

TEST(Synthesis, MappingRW) {

}

TEST(Synthesis, SubstitutionDummy) {
    EXPECT_THROW(synthesize(Substitution("0 2 1")), SynthException);
    {
        Substitution sub("0 1");
        Circuit c("Lines: 2; 1\nkCNOT(1; 0)");
        EXPECT_EQ(synthesize(sub, "dummy"), dummy_algorithm(sub));
        EXPECT_EQ(dummy_algorithm(sub), c);
    }
    {
        Substitution sub("1 0");
        Circuit c("Lines: 2; 1\nNOT(1)\nkCNOT(1; 0)");
        EXPECT_EQ(synthesize(sub, "dummy"), dummy_algorithm(sub));
        EXPECT_EQ(dummy_algorithm(sub), c);
    }
    {
        Substitution sub("3 2 0 1");
        Circuit c("Lines: 4; 2\n"
                  "NOT(2)\nkCNOT(2; 0)\n"
                  "NOT(3)\nkCNOT(3; 1)\nkCNOT(3; 0)");
        EXPECT_EQ(synthesize(sub, "dummy"), dummy_algorithm(sub));
        EXPECT_EQ(dummy_algorithm(sub), c);
    }
    {
        Substitution sub("5 7 2 1 6 3 0 4");
        Circuit c("Lines: 6; 3\n"
                  "NOT(3)\nkCNOT(3; 1)\nkCNOT(3; 0, 2)\n"
                  "kCNOT(4; 2)\nkCNOT(4; 1)\nkCNOT(4; 0)\nkCNOT(4; 0, 2)\n"
                  "NOT(5)\nkCNOT(5; 1)\nkCNOT(5; 1, 2)\nkCNOT(5; 0)\nkCNOT(5; 0, 2)\nkCNOT(5; 0, 1)\n");
        EXPECT_EQ(synthesize(sub, "dummy"), dummy_algorithm(sub));
        EXPECT_EQ(dummy_algorithm(sub), c);
    }
}

TEST(Synthesis, SubstitutionRW) {
    EXPECT_THROW(synthesize(Substitution("0 2 1")), SynthException);
    {
        Substitution sub("0 1 2 3");
        Circuit c("Lines: 2");
        EXPECT_EQ(synthesize(sub, "rw"), RW_algorithm(sub));
        EXPECT_EQ(Substitution(RW_algorithm(sub).produce_mapping()), sub);
    }
    {
        Substitution sub("0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15");
        Circuit c("Lines: 4");
        EXPECT_EQ(synthesize(sub, "rw"), RW_algorithm(sub));
        EXPECT_EQ(RW_algorithm(sub).produce_mapping(), sub);
    }
    {
        Substitution sub("0 1 2 3 4 6 5 7");
        Circuit c("Lines: 3\nCNOT(2; 1)\nkCNOT(1; 0, 2)\nCNOT(2; 1)");
        EXPECT_EQ(synthesize(sub, "rw"), RW_algorithm(sub));
        EXPECT_EQ(RW_algorithm(sub).produce_mapping(), sub);
    }

//    {
//        Substitution sub("0 1 2 4 3 5 6 7");
//        Circuit c("Lines: 3\nCNOT(2; 0)\nCNOT(1; 0)\nkCNOT(0; 1, 2)\nCNOT(2; 0)\nCNOT(1; 0)\n");
//        EXPECT_EQ(synthesize(sub, "rw"), RW_algorithm(sub));
//        EXPECT_EQ(RW_algorithm(sub).produce_mapping(), sub);
//    }
//    {
//        Substitution sub("0 1 2 3 4 5 6 8 7 9 10 11 12 13 14 15");
//        Circuit c("Lines: 4\nCNOT(3; 0)\nCNOT(2; 0)\nCNOT(1; 0)\n"
//                  "kCNOT(0; 1, 2, 3)\nCNOT(3; 0)\nCNOT(2; 0)\nCNOT(1; 0)\n");
//        EXPECT_EQ(synthesize(sub, "rw"), RW_algorithm(sub));
//        EXPECT_EQ(RW_algorithm(sub).produce_mapping(), sub);
//    }
//    {
//        Substitution sub("1 2 3 4 5 6 7 0");
//        Circuit c("Lines: 3\nkCNOT(0; 1, 2)\nNOT(2)\nNOT(1)\nCNOT(1; 2)\n");
//        EXPECT_EQ(synthesize(sub, "rw"), RW_algorithm(sub));
//        EXPECT_EQ(RW_algorithm(sub).produce_mapping(), sub);
//    }
//    {
//        Substitution sub("1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 0");
//        Circuit c("Lines: 4\nkCNOT(0; 1, 2, 3)\nkCNOT(1; 2, 3)\nNOT(3)\nNOT(2)\nCNOT(2; 3)");
//        EXPECT_EQ(synthesize(sub, "rw"), RW_algorithm(sub));
//        EXPECT_EQ(RW_algorithm(sub).produce_mapping(), sub);
//    }
//    {
//        Substitution sub("3 11 2 10 0 7 1 6 15 8 14 9 13 5 12 4");
//        Circuit c("Lines: 4\nNOT(3)\nNOT(1)\nkCNOT(3; 1, 2)\nNOT(1)\n"
//                  "kCNOT(3; 0, 2)\nNOT(2)\nkCNOT(3; 0, 1)\nCNOT(2; 0)\nkCNOT(0; 1, 2)\n"
//                  "CNOT(2; 1)\nCNOT(2; 0)\nCNOT(1; 0)\n");
//        EXPECT_EQ(synthesize(sub, "rw"), RW_algorithm(sub));
//        EXPECT_EQ(RW_algorithm(sub).produce_mapping(), sub);
//    }
//    {
//        Substitution sub("4 6 2 0 15 13 7 5 9 11 3 1 14 12 10 8");
//        Circuit c("Lines: 4\nNOT(2)\nNOT(1)\nNOT(0)\nkCNOT(2; 0, 1)\nNOT(0)\nCNOT(3; 0)\nCNOT(1; 0)\nkCNOT(0; 1, 3)\nCNOT(1; 0)");
//        EXPECT_EQ(synthesize(sub, "rw"), RW_algorithm(sub));
//        EXPECT_EQ(RW_algorithm(sub).produce_mapping(), sub);
//    }
}
