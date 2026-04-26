#include <gtest/gtest.h>

#include "synthesis.hpp"

TEST(Synthesis, MappingCA) {
    JobsConfig::instance().set(std::thread::hardware_concurrency());

    {
        BinaryMapping bm(table{{1, 0}});
        Circuit c = CA_algorithm(bm);
        EXPECT_EQ(synthesize(bm, Algo::CA), c);
        EXPECT_EQ(synthesize(bm, Algo::CA, true), c);
        EXPECT_EQ(c.produce_mapping().coordinate_functions().front(), BooleanFunction("10"));
        EXPECT_EQ(c.memory(), 0);
    }
    {
        BinaryMapping bm(table{{1, 1}});
        Circuit c = CA_algorithm(bm);
        EXPECT_EQ(synthesize(bm, Algo::CA), c);
        EXPECT_EQ(synthesize(bm, Algo::CA, true), c);
        EXPECT_EQ(c.produce_mapping().coordinate_functions().front(), BooleanFunction("01"));
        EXPECT_EQ(c.memory(), 1);
    }
    {
        BinaryMapping bm(table{{0, 1, 1, 0, 1, 1, 1, 1}});
        Circuit c = CA_algorithm(bm);
        EXPECT_EQ(synthesize(bm, Algo::CA), c);
        EXPECT_EQ(synthesize(bm, Algo::CA, true), c);
        EXPECT_EQ(c.produce_mapping().coordinate_functions().back(), BooleanFunction("01101111"));
        EXPECT_EQ(c.memory(), 1);
    }
    {
        BinaryMapping bm(table{{0, 1},
                               {1, 0},
                               {0, 0},
                               {1, 1}});
        Circuit c = CA_algorithm(bm);
        EXPECT_EQ(synthesize(bm, Algo::CA), c);
        EXPECT_EQ(synthesize(bm, Algo::CA, true), c);
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
        Circuit c = CA_algorithm(bm);
        EXPECT_EQ(synthesize(bm, Algo::CA), c);
        EXPECT_EQ(synthesize(bm, Algo::CA, true), c);
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
        Circuit c = CA_algorithm(bm);
        EXPECT_EQ(synthesize(bm, Algo::CA), c);
        EXPECT_EQ(synthesize(bm, Algo::CA, true), c);
        EXPECT_EQ(c.produce_mapping().coordinate_functions().back(), BooleanFunction("0011"));
        EXPECT_EQ(c.memory(), 0);
    }
    {
        BinaryMapping bm(table{{0, 0, 1, 1},
                               {0, 1, 0, 1}});
        Circuit c = CA_algorithm(bm);
        EXPECT_EQ(synthesize(bm, Algo::CA), c);
        EXPECT_EQ(synthesize(bm, Algo::CA, true), c);
        EXPECT_EQ(c.produce_mapping(), bm);
        EXPECT_EQ(c.memory(), 0);
    }
    {
        BinaryMapping bm(table{{0, 0, 0, 0, 1, 1, 1, 1},
                               {0, 0, 1, 1, 0, 0, 1, 1},
                               {0, 1, 0, 1, 0, 1, 0, 1}});
        Circuit c = CA_algorithm(bm);
        EXPECT_EQ(synthesize(bm, Algo::CA), c);
        EXPECT_EQ(synthesize(bm, Algo::CA, true), c);
        EXPECT_EQ(c.produce_mapping(), bm);
        EXPECT_EQ(c.memory(), 0);
    }
}

TEST(Synthesis, SubstitutionCA) {
    JobsConfig::instance().set(std::thread::hardware_concurrency());

    EXPECT_THROW(CA_algorithm(Substitution("0 2 1")), SynthException);
    {
        Substitution sub("2 3 1 0");
        Circuit c = CA_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::CA), c);
        EXPECT_EQ(synthesize(sub, Algo::CA, true), c);
        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("1 2 3 0");
        Circuit c = CA_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::CA), c);
        EXPECT_EQ(synthesize(sub, Algo::CA, true), c);
        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("3 0 6 1 4 7 2 5");
        Circuit c = CA_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::CA), c);
        EXPECT_EQ(synthesize(sub, Algo::CA, true), c);
//        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("3 5 7 6 0 1 4 2");
        Circuit c = CA_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::CA), c);
        EXPECT_EQ(synthesize(sub, Algo::CA, true), c);
//        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("5 3 0 1 7 6 2 4");
        Circuit c = CA_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::CA), c);
        EXPECT_EQ(synthesize(sub, Algo::CA, true), c);
//        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("6 5 3 4 2 1 7 0");
        Circuit c = CA_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::CA), c);
        EXPECT_EQ(synthesize(sub, Algo::CA, true), c);
//        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("7 5 3 4 2 1 0 6");
        Circuit c = CA_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::CA), c);
        EXPECT_EQ(synthesize(sub, Algo::CA, true), c);
//        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("7 6 5 4 3 2 0 1");
        Circuit c = CA_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::CA), c);
        EXPECT_EQ(synthesize(sub, Algo::CA, true), c);
//        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("1 10 5 7 14 3 13 6 0 9 11 4 15 8 12 2");
        Circuit c = CA_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::CA), c);
        EXPECT_EQ(synthesize(sub, Algo::CA, true), c);
//        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("3 11 14 13 10 8 4 9 1 0 15 6 12 2 7 5");
        Circuit c = CA_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::CA), c);
        EXPECT_EQ(synthesize(sub, Algo::CA, true), c);
//        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("21 27 23 8 6 11 5 18 22 26 7 13 12 28 20 4 1 2 9 14 16 17 15 24 19 10 25 31 0 3 30 29");
        Circuit c = CA_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::CA), c);
        EXPECT_EQ(synthesize(sub, Algo::CA, true), c);
//        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("68 34 115 79 116 71 78 75 90 6 57 103 60 21 74 66 46 9 96 98 104 100 42 32 50 33 58 117 2 12 "
                         "44 119 81 83 106 41 19 56 22 55 61 13 52 27 25 107 17 127 124 26 18 29 3 38 123 84 76 87 93 "
                         "85 73 99 67 92 28 8 14 30 95 45 110 10 37 111 109 20 53 15 11 97 7 91 43 89 120 65 102 118 "
                         "62 39 64 54 113 114 101 126 40 86 88 112 47 82 122 69 125 49 36 121 0 23 72 24 48 51 94 16 "
                         "31 1 80 4 35 77 59 63 108 70 105 5");
        Circuit c = CA_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::CA), c);
        EXPECT_EQ(synthesize(sub, Algo::CA, true), c);
//        EXPECT_EQ(c.produce_mapping(), sub);
    }
}
