#include <gtest/gtest.h>

#include "synthesis.hpp"


TEST(Synthesis, MappingZKB) {
    JobsConfig::instance().set(std::thread::hardware_concurrency());

    {
        BinaryMapping bm(table{{0, 1, 1, 0, 1, 1, 1, 1}});
        Circuit c = ZKB_algorithm(bm);
        EXPECT_EQ(synthesize(bm, Algo::ZKB), c);
        EXPECT_EQ(synthesize(bm, Algo::ZKB, true), c);
        EXPECT_EQ(c.produce_mapping().coordinate_functions().back(), BooleanFunction("01101111"));
        EXPECT_EQ(c.memory(), 1);
    }
    {
        BinaryMapping bm(table{{0, 1},
                               {1, 0},
                               {0, 0},
                               {1, 1}});
        Circuit c = ZKB_algorithm(bm);
        EXPECT_EQ(synthesize(bm, Algo::ZKB), c);
        EXPECT_EQ(synthesize(bm, Algo::ZKB, true), c);
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
        Circuit c = ZKB_algorithm(bm);
        EXPECT_EQ(synthesize(bm, Algo::ZKB), c);
        EXPECT_EQ(synthesize(bm, Algo::ZKB, true), c);
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
        BinaryMapping bm(table{{0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1},
                               {0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0},
                               {1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1},
                               {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0}});
        Circuit c = ZKB_algorithm(bm);
        EXPECT_EQ(synthesize(bm, Algo::ZKB), c);
        EXPECT_EQ(synthesize(bm, Algo::ZKB, true), c);
        EXPECT_EQ(c.produce_mapping(), bm);
        EXPECT_EQ(c.memory(), 0);
    }
}

TEST(Synthesis, SubstitutionZKB) {
    JobsConfig::instance().set(std::thread::hardware_concurrency());

    EXPECT_THROW(ZKB_algorithm(Substitution("0 2 1")), SynthException);
    EXPECT_THROW(ZKB_algorithm(Substitution("1 0")), SynthException);
    EXPECT_THROW(ZKB_algorithm(Substitution("0 2 1 3")), SynthException);
    EXPECT_THROW(ZKB_algorithm(Substitution("0 7 2 6 1 4 3 5")), SynthException);

    {
        Substitution sub("0 1 2 3 4 5 6 7 8 9 A B C D E F");
        Circuit c = ZKB_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::ZKB), c);
        EXPECT_EQ(synthesize(sub, Algo::ZKB, true), c);
        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("0 1 2 3 4 5 7 6 8 9 A B C D E F");
        Circuit c = ZKB_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::ZKB), c);
        EXPECT_EQ(synthesize(sub, Algo::ZKB, true), c);
        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("0 1 E 3 4 5 6 7 8 9 A B C D 2 F");
        Circuit c = ZKB_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::ZKB), c);
        EXPECT_EQ(synthesize(sub, Algo::ZKB, true), c);
        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("1 0 3 2 5 4 7 6 9 8 B A D C F E");
        Circuit c = ZKB_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::ZKB), c);
        EXPECT_EQ(synthesize(sub, Algo::ZKB, true), c);
        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("0 1 2 3 4 5 6 8 7 9 A B C D E F");
        Circuit c = ZKB_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::ZKB), c);
        EXPECT_EQ(synthesize(sub, Algo::ZKB, true), c);
        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("1 2 3 4 5 6 7 8 9 A B C D E F 0");
        Circuit c = ZKB_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::ZKB), c);
        EXPECT_EQ(synthesize(sub, Algo::ZKB, true), c);
        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("F 0 1 2 3 4 5 6 7 8 9 A B C D E");
        Circuit c = ZKB_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::ZKB), c);
        EXPECT_EQ(synthesize(sub, Algo::ZKB, true), c);
        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("F 1 2 3 4 5 6 7 8 9 A B C D E 0");
        Circuit c = ZKB_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::ZKB), c);
        EXPECT_EQ(synthesize(sub, Algo::ZKB, true), c);
        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("3 B 2 A 0 7 1 6 F 8 E 9 D 5 C 4");
        Circuit c = ZKB_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::ZKB), c);
        EXPECT_EQ(synthesize(sub, Algo::ZKB, true), c);
        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("4 6 2 0 F D 7 5 9 B 3 1 E C A 8");
        Circuit c = ZKB_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::ZKB), c);
        EXPECT_EQ(synthesize(sub, Algo::ZKB, true), c);
        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        // NOT(2)
        Substitution sub("2 3 0 1 6 7 4 5 A B 8 9 E F C D");
        Circuit c = ZKB_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::ZKB), c);
        EXPECT_EQ(synthesize(sub, Algo::ZKB, true), c);
        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("19 25 18 2 3 17 0 1 16 27 6 31 7 26 4 24 5 28 10 20 8 13 9 14 15 29 21 11 30 12 22 23");
        Circuit c = ZKB_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::ZKB), c);
        EXPECT_EQ(synthesize(sub, Algo::ZKB, true), c);
        EXPECT_EQ(c.produce_mapping(), sub);
    }
}
