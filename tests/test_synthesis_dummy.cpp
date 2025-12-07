#include <gtest/gtest.h>

#include "synthesis.hpp"


TEST(Synthesis, MappingDummy) {
    JobsConfig::instance().set(std::thread::hardware_concurrency());

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

TEST(Synthesis, SubstitutionDummy) {
    JobsConfig::instance().set(std::thread::hardware_concurrency());

    EXPECT_THROW(dummy_algorithm(Substitution("0 2 1")), SynthException);
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
    {
        Substitution sub("2 5 4 1 0 6 7 3");
        Circuit c = dummy_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::DUMMY), c);
        EXPECT_EQ(synthesize(sub, Algo::DUMMY, true), c);
    }
    {
        Substitution sub("3 6 9 4 11 0 15 13 1 14 12 8 2 5 10 7");
        Circuit c = dummy_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::DUMMY), c);
        EXPECT_EQ(synthesize(sub, Algo::DUMMY, true), c);
    }
    {
        Substitution sub("29 23 25 10 31 30 9 12 2 19 0 20 24 26 14 1 11 22 21 5 27 6 17 4 16 8 18 28 15 13 7 3");
        Circuit c = dummy_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::DUMMY), c);
        EXPECT_EQ(synthesize(sub, Algo::DUMMY, true), c);
    }
    {
        Substitution sub(
                "121 103 19 105 85 88 16 50 78 109 83 63 124 42 56 33 55 38 59 34 94 5 97 24 25 9 82 28 61 52 53 112 "
                "104 86 43 100 96 120 8 73 2 117 66 20 15 35 46 21 90 26 74 95 12 119 93 113 10 57 127 91 32 81 36 64 "
                "30 108 44 51 69 47 118 22 60 101 13 58 48 98 75 29 23 70 18 72 89 37 77 54 102 31 87 27 111 39 116 "
                "123 41 67 110 71 7 126 4 40 106 0 3 80 65 125 115 6 79 14 114 68 76 11 17 1 49 99 84 107 62 45 122 "
                "92");
        Circuit c = dummy_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::DUMMY), c);
        EXPECT_EQ(synthesize(sub, Algo::DUMMY, true), c);
    }
}
