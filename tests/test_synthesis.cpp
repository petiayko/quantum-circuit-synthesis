#include <gtest/gtest.h>

#include "synthesis.hpp"


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

TEST(Synthesis, MappingDummy) {
    {
        BinaryMapping bm(table{{0, 0}});
        Circuit c("Lines: 2; 1");
        EXPECT_EQ(dummy_algorithm(bm), c);
        EXPECT_EQ(synthesize(bm, Algo::DUMMY), c);
        EXPECT_EQ(synthesize(bm, Algo::DUMMY, true), c);
    }
    {
        BinaryMapping bm(table{{1, 1}});
        Circuit c("Lines: 2; 1\nNOT(1)");
        EXPECT_EQ(dummy_algorithm(bm), c);
        EXPECT_EQ(synthesize(bm, Algo::DUMMY), c);
        EXPECT_EQ(synthesize(bm, Algo::DUMMY, true), c);
    }
    {
        BinaryMapping bm(table{{0, 0, 0, 0, 0, 0, 0, 0}});
        Circuit c("Lines: 4; 1");
        EXPECT_EQ(dummy_algorithm(bm), c);
        EXPECT_EQ(synthesize(bm, Algo::DUMMY), c);
        EXPECT_EQ(synthesize(bm, Algo::DUMMY, true), c);
    }
    {
        BinaryMapping bm(table{{1, 1, 1, 1, 1, 1, 1, 1}});
        Circuit c("Lines: 4; 1\nNOT(3)");
        EXPECT_EQ(dummy_algorithm(bm), c);
        EXPECT_EQ(synthesize(bm, Algo::DUMMY), c);
        EXPECT_EQ(synthesize(bm, Algo::DUMMY, true), c);
    }
    {
        BinaryMapping bm(table{{1, 0, 0, 0}});
        Circuit c("Lines: 3; 1\nNOT(2)\nkCNOT(2; 1)\nkCNOT(2; 0)\nkCNOT(2; 0, 1)");
        EXPECT_EQ(dummy_algorithm(bm), c);
        EXPECT_EQ(synthesize(bm, Algo::DUMMY), c);
        EXPECT_EQ(synthesize(bm, Algo::DUMMY, true), c);
    }
    {
        BinaryMapping bm(table{{1, 0, 0, 0},
                               {0, 0, 1, 0}});
        Circuit c("Lines: 4; 2\n"
                  "NOT(2)\nkCNOT(2; 1)\nkCNOT(2; 0)\nkCNOT(2; 0, 1)\n"
                  "kCNOT(3; 0)\nkCNOT(3; 0, 1)");
        EXPECT_EQ(dummy_algorithm(bm), c);
        EXPECT_EQ(synthesize(bm, Algo::DUMMY), c);
        EXPECT_EQ(synthesize(bm, Algo::DUMMY, true), c);
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
        EXPECT_EQ(dummy_algorithm(bm), c);
        EXPECT_EQ(synthesize(bm, Algo::DUMMY), c);
        EXPECT_EQ(synthesize(bm, Algo::DUMMY, true), c);
    }
}

TEST(Synthesis, MappingRW) {
    {
        BinaryMapping bm(Substitution("0 1 2 3"));
        Circuit c = RW_algorithm(bm);
        EXPECT_EQ(c.produce_mapping(), bm);
        EXPECT_EQ(synthesize(bm, Algo::RW), c);
        EXPECT_EQ(synthesize(bm, Algo::RW, true), c);
    }
    {
        BinaryMapping bm(Substitution("0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15"));
        Circuit c = RW_algorithm(bm);
        EXPECT_EQ(c.produce_mapping(), bm);
        EXPECT_EQ(synthesize(bm, Algo::RW), c);
        EXPECT_EQ(synthesize(bm, Algo::RW, true), c);
    }
    {
        BinaryMapping bm(Substitution("0 1 2 3 4 6 5 7"));
        Circuit c = RW_algorithm(bm);
        EXPECT_EQ(c.produce_mapping(), bm);
        EXPECT_EQ(synthesize(bm, Algo::RW), c);
        EXPECT_EQ(synthesize(bm, Algo::RW, true), c);
    }
    {
        BinaryMapping bm(Substitution("0 1 2 4 3 5 6 7"));
        Circuit c = RW_algorithm(bm);
        EXPECT_EQ(c.produce_mapping(), bm);
        EXPECT_EQ(synthesize(bm, Algo::RW), c);
        EXPECT_EQ(synthesize(bm, Algo::RW, true), c);
    }
    {
        BinaryMapping bm(Substitution("0 1 2 3 4 5 6 8 7 9 10 11 12 13 14 15"));
        Circuit c = RW_algorithm(bm);
        EXPECT_EQ(c.produce_mapping(), bm);
        EXPECT_EQ(synthesize(bm, Algo::RW), c);
        EXPECT_EQ(synthesize(bm, Algo::RW, true), c);
    }
    {
        BinaryMapping bm(Substitution("1 2 3 4 5 6 7 0"));
        Circuit c = RW_algorithm(bm);
        EXPECT_EQ(c.produce_mapping(), bm);
        EXPECT_EQ(synthesize(bm, Algo::RW), c);
        EXPECT_EQ(synthesize(bm, Algo::RW, true), c);
    }
    {
        BinaryMapping bm(Substitution("7 1 2 3 4 5 6 0"));
        Circuit c = RW_algorithm(bm);
        EXPECT_EQ(c.produce_mapping(), bm);
        EXPECT_EQ(synthesize(bm, Algo::RW), c);
        EXPECT_EQ(synthesize(bm, Algo::RW, true), c);
    }
    {
        BinaryMapping bm(Substitution("7 0 1 2 3 4 5 6"));
        Circuit c = RW_algorithm(bm);
        EXPECT_EQ(c.produce_mapping(), bm);
        EXPECT_EQ(synthesize(bm, Algo::RW), c);
        EXPECT_EQ(synthesize(bm, Algo::RW, true), c);
    }
    {
        BinaryMapping bm(Substitution("1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 0"));
        Circuit c = RW_algorithm(bm);
        EXPECT_EQ(c.produce_mapping(), bm);
        EXPECT_EQ(synthesize(bm, Algo::RW), c);
        EXPECT_EQ(synthesize(bm, Algo::RW, true), c);
    }
    {
        BinaryMapping bm(Substitution("15 1 2 3 4 5 6 7 8 9 10 11 12 13 14 0"));
        Circuit c = RW_algorithm(bm);
        EXPECT_EQ(c.produce_mapping(), bm);
        EXPECT_EQ(synthesize(bm, Algo::RW), c);
        EXPECT_EQ(synthesize(bm, Algo::RW, true), c);
    }
    {
        BinaryMapping bm(Substitution("3 11 2 10 0 7 1 6 15 8 14 9 13 5 12 4"));
        Circuit c = RW_algorithm(bm);
        EXPECT_EQ(c.produce_mapping(), bm);
        EXPECT_EQ(synthesize(bm, Algo::RW), c);
        EXPECT_EQ(synthesize(bm, Algo::RW, true), c);
    }
    {
        BinaryMapping bm(Substitution("4 6 2 0 15 13 7 5 9 11 3 1 14 12 10 8"));
        // kill me for this too
        EXPECT_THROW(synthesize(bm, Algo::RW), SynthException);
        EXPECT_THROW(RW_algorithm(bm), SynthException);
    }
    {
        BinaryMapping bm(Substitution("0 1 3 2 5 4 6 7 9 8 11 10 13 12 15 14"));
        Circuit c = RW_algorithm(bm);
        EXPECT_EQ(c.produce_mapping(), bm);
        EXPECT_EQ(synthesize(bm, Algo::RW), c);
        EXPECT_EQ(synthesize(bm, Algo::RW, true), c);
    }
    {
        BinaryMapping bm(table{{0, 1, 1, 0, 1, 1, 1, 1}});
        Circuit c = RW_algorithm(bm);
        EXPECT_EQ(synthesize(bm, Algo::RW), c);
        EXPECT_EQ(synthesize(bm, Algo::RW, true), c);
        EXPECT_EQ(c.produce_mapping().coordinate_functions().back(), BooleanFunction("01101111"));
        EXPECT_EQ(c.memory(), 1);
    }
    {
        BinaryMapping bm(table{{0, 1},
                               {1, 0},
                               {0, 0},
                               {1, 1}});
        Circuit c = RW_algorithm(bm);
        EXPECT_EQ(synthesize(bm, Algo::RW), c);
        EXPECT_EQ(synthesize(bm, Algo::RW, true), c);
        EXPECT_EQ(c.produce_mapping(), BinaryMapping(table{{0, 1},
                                                           {0, 1},
                                                           {1, 0},
                                                           {0, 0},
                                                           {1, 1}}));
        EXPECT_EQ(c.memory(), 4);
    }
    {
        BinaryMapping bm(table{{0, 1, 1, 1},
                               {0, 0, 0, 0},
                               {1, 0, 0, 1},
                               {0, 1, 0, 0},
                               {1, 1, 0, 1}});
        Circuit c = RW_algorithm(bm);
        EXPECT_EQ(synthesize(bm, Algo::RW), c);
        EXPECT_EQ(synthesize(bm, Algo::RW, true), c);
        EXPECT_EQ(c.produce_mapping(), BinaryMapping(table{{0, 0, 1, 1},
                                                           {0, 1, 0, 1},
                                                           {0, 1, 1, 1},
                                                           {0, 0, 0, 0},
                                                           {1, 0, 0, 1},
                                                           {0, 1, 0, 0},
                                                           {1, 1, 0, 1}}));
        EXPECT_EQ(c.memory(), 5);
    }
    {
        BinaryMapping bm(table{{0, 0, 1, 1}});
        Circuit c = RW_algorithm(bm);
        EXPECT_EQ(synthesize(bm, Algo::RW), c);
        EXPECT_EQ(synthesize(bm, Algo::RW, true), c);
        EXPECT_EQ(c.produce_mapping().coordinate_functions().back(), BooleanFunction("0011"));
        EXPECT_EQ(c.memory(), 0);
    }
    {
        BinaryMapping bm(table{{0, 0, 1, 1},
                               {0, 1, 0, 1}});
        Circuit c = RW_algorithm(bm);
        EXPECT_EQ(synthesize(bm, Algo::RW), c);
        EXPECT_EQ(synthesize(bm, Algo::RW, true), c);
        EXPECT_EQ(c.produce_mapping(), bm);
        EXPECT_EQ(c.memory(), 0);
    }
    {
        BinaryMapping bm(table{{0, 0, 0, 0, 1, 1, 1, 1},
                               {0, 0, 1, 1, 0, 0, 1, 1},
                               {0, 1, 0, 1, 0, 1, 0, 1}});
        Circuit c = RW_algorithm(bm);
        EXPECT_EQ(synthesize(bm, Algo::RW), c);
        EXPECT_EQ(synthesize(bm, Algo::RW, true), c);
        EXPECT_EQ(c.produce_mapping(), bm);
        EXPECT_EQ(c.memory(), 0);
    }
}

TEST(Synthesis, SubstitutionDummy) {
    EXPECT_THROW(synthesize(Substitution("0 2 1")), SynthException);
    {
        Substitution sub("0 1");
        Circuit c_etalon("Lines: 2; 1\nkCNOT(1; 0)");
        Circuit c = dummy_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::DUMMY), c);
        EXPECT_EQ(synthesize(sub, Algo::DUMMY, true), c);
        EXPECT_EQ(c_etalon, c);
    }
    {
        Substitution sub("1 0");
        Circuit c_etalon("Lines: 2; 1\nNOT(1)\nkCNOT(1; 0)");
        Circuit c = dummy_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::DUMMY), c);
        EXPECT_EQ(synthesize(sub, Algo::DUMMY, true), c);
        EXPECT_EQ(c_etalon, c);
    }
    {
        Substitution sub("3 2 0 1");
        Circuit c_etalon("Lines: 4; 2\n"
                         "NOT(2)\nkCNOT(2; 0)\n"
                         "NOT(3)\nkCNOT(3; 1)\nkCNOT(3; 0)");
        Circuit c = dummy_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::DUMMY), c);
        EXPECT_EQ(synthesize(sub, Algo::DUMMY, true), c);
        EXPECT_EQ(c_etalon, c);
    }
    {
        Substitution sub("5 7 2 1 6 3 0 4");
        Circuit c_etalon("Lines: 6; 3\n"
                         "NOT(3)\nkCNOT(3; 1)\nkCNOT(3; 0, 2)\n"
                         "kCNOT(4; 2)\nkCNOT(4; 1)\nkCNOT(4; 0)\nkCNOT(4; 0, 2)\n"
                         "NOT(5)\nkCNOT(5; 1)\nkCNOT(5; 1, 2)\nkCNOT(5; 0)\nkCNOT(5; 0, 2)\nkCNOT(5; 0, 1)\n");
        Circuit c = dummy_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::DUMMY), c);
        EXPECT_EQ(synthesize(sub, Algo::DUMMY, true), c);
        EXPECT_EQ(c_etalon, c);
    }
}

TEST(Synthesis, SubstitutionRW) {
    EXPECT_THROW(synthesize(Substitution("0 2 1")), SynthException);
    {
        Substitution sub("0 1 2 3");
        Circuit c = RW_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::RW), c);
        EXPECT_EQ(synthesize(sub, Algo::RW, true), c);
        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15");
        Circuit c = RW_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::RW), c);
        EXPECT_EQ(synthesize(sub, Algo::RW, true), c);
        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("0 1 2 3 4 6 5 7");
        Circuit c = RW_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::RW), c);
        EXPECT_EQ(synthesize(sub, Algo::RW, true), c);
        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("0 1 2 4 3 5 6 7");
        Circuit c = RW_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::RW), c);
        EXPECT_EQ(synthesize(sub, Algo::RW, true), c);
        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("0 1 2 3 4 5 6 8 7 9 10 11 12 13 14 15");
        Circuit c = RW_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::RW), c);
        EXPECT_EQ(synthesize(sub, Algo::RW, true), c);
        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("1 2 3 4 5 6 7 0");
        Circuit c = RW_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::RW), c);
        EXPECT_EQ(synthesize(sub, Algo::RW, true), c);
        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("7 1 2 3 4 5 6 0");
        Circuit c = RW_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::RW), c);
        EXPECT_EQ(synthesize(sub, Algo::RW, true), c);
        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("7 0 1 2 3 4 5 6");
        Circuit c = RW_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::RW), c);
        EXPECT_EQ(synthesize(sub, Algo::RW, true), c);
        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 0");
        Circuit c = RW_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::RW), c);
        EXPECT_EQ(synthesize(sub, Algo::RW, true), c);
        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("15 1 2 3 4 5 6 7 8 9 10 11 12 13 14 0");
        Circuit c = RW_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::RW), c);
        EXPECT_EQ(synthesize(sub, Algo::RW, true), c);
        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("3 11 2 10 0 7 1 6 15 8 14 9 13 5 12 4");
        Circuit c = RW_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::RW), c);
        EXPECT_EQ(synthesize(sub, Algo::RW, true), c);
        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("4 6 2 0 15 13 7 5 9 11 3 1 14 12 10 8");
        // kill me for this
        EXPECT_THROW(synthesize(sub, Algo::RW), SynthException);
        EXPECT_THROW(RW_algorithm(sub), SynthException);
    }
}
