#include <gtest/gtest.h>

#include "synthesis.hpp"


TEST(Synthesis, MappingSS) {
    JobsConfig::instance().set(std::thread::hardware_concurrency());

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
        EXPECT_THROW(synthesize(bm, Algo::GS), SynthException);
        EXPECT_THROW(GS_algorithm(bm), SynthException);
    }
    {
        BinaryMapping bm(table{{0, 1, 1, 1},
                               {0, 0, 0, 0},
                               {1, 0, 0, 1},
                               {0, 1, 0, 0},
                               {1, 1, 0, 1}});
        EXPECT_THROW(synthesize(bm, Algo::GS), SynthException);
        EXPECT_THROW(GS_algorithm(bm), SynthException);
    }
    {
        BinaryMapping bm(table{{0, 0, 1, 1}});
        Circuit c = GS_algorithm(bm);
        EXPECT_EQ(synthesize(bm, Algo::GS), c);
        EXPECT_EQ(synthesize(bm, Algo::GS, true), c);
        EXPECT_EQ(c.produce_mapping().coordinate_functions().back(), BooleanFunction("0011"));
        EXPECT_EQ(c.memory(), 0);
    }
    {
        BinaryMapping bm(table{{0, 0, 1, 1},
                               {0, 1, 0, 1}});
        Circuit c = GS_algorithm(bm);
        EXPECT_EQ(synthesize(bm, Algo::GS), c);
        EXPECT_EQ(synthesize(bm, Algo::GS, true), c);
        EXPECT_EQ(c.produce_mapping(), bm);
        EXPECT_EQ(c.memory(), 0);
    }
    {
        BinaryMapping bm(table{{0, 0, 0, 0, 1, 1, 1, 1},
                               {0, 0, 1, 1, 0, 0, 1, 1},
                               {0, 1, 0, 1, 0, 1, 0, 1}});
        Circuit c = GS_algorithm(bm);
        EXPECT_EQ(synthesize(bm, Algo::GS), c);
        EXPECT_EQ(synthesize(bm, Algo::GS, true), c);
        EXPECT_EQ(c.produce_mapping(), bm);
        EXPECT_EQ(c.memory(), 0);
    }
}

TEST(Synthesis, SubstitutionSS) {
    JobsConfig::instance().set(std::thread::hardware_concurrency());

    EXPECT_THROW(GS_algorithm(Substitution("0 2 1")), SynthException);
    {
        Substitution sub("0 1 2 3");
        Circuit c = GS_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::GS), c);
        EXPECT_EQ(synthesize(sub, Algo::GS, true), c);
        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("0 2 3 1");
        Circuit c = GS_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::GS), c);
        EXPECT_EQ(synthesize(sub, Algo::GS, true), c);
        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15");
        Circuit c = GS_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::GS), c);
        EXPECT_EQ(synthesize(sub, Algo::GS, true), c);
        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("0 1 2 3 4 6 5 7");
        Circuit c = GS_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::GS), c);
        EXPECT_EQ(synthesize(sub, Algo::GS, true), c);
        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("0 1 2 4 3 5 6 7");
        EXPECT_THROW(synthesize(sub, Algo::GS), SynthException);
        EXPECT_THROW(GS_algorithm(sub), SynthException);
    }
    {
        Substitution sub("0 1 2 3 4 5 6 8 7 9 10 11 12 13 14 15");
        EXPECT_THROW(synthesize(sub, Algo::GS), SynthException);
        EXPECT_THROW(GS_algorithm(sub), SynthException);
    }
    {
        Substitution sub("1 2 3 4 5 6 7 0");
        Circuit c = GS_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::GS), c);
        EXPECT_EQ(synthesize(sub, Algo::GS, true), c);
        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("7 1 2 3 4 5 6 0");
        EXPECT_THROW(synthesize(sub, Algo::GS), SynthException);
        EXPECT_THROW(GS_algorithm(sub), SynthException);
    }
    {
        Substitution sub("7 0 1 2 3 4 5 6");
        Circuit c = GS_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::GS), c);
        EXPECT_EQ(synthesize(sub, Algo::GS, true), c);
        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 0");
        Circuit c = GS_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::GS), c);
        EXPECT_EQ(synthesize(sub, Algo::GS, true), c);
        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("15 1 2 3 4 5 6 7 8 9 10 11 12 13 14 0");
        EXPECT_THROW(synthesize(sub, Algo::GS), SynthException);
        EXPECT_THROW(GS_algorithm(sub), SynthException);
    }
    {
        Substitution sub("3 11 2 10 0 7 1 6 15 8 14 9 13 5 12 4");
        EXPECT_THROW(synthesize(sub, Algo::GS), SynthException);
        EXPECT_THROW(GS_algorithm(sub), SynthException);
    }
    {
        Substitution sub("4 6 2 0 15 13 7 5 9 11 3 1 14 12 10 8");
        EXPECT_THROW(synthesize(sub, Algo::GS), SynthException);
        EXPECT_THROW(GS_algorithm(sub), SynthException);
    }
}
