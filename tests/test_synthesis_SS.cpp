#include <gtest/gtest.h>

#include "synthesis.hpp"


TEST(Synthesis, MappingSS) {
}

TEST(Synthesis, SubstitutionSS) {
    EXPECT_THROW(SS_algorithm(Substitution("0 2 1")), SynthException);
    {
        Substitution sub("0 1 2 3");
        Circuit c = SS_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::SS), c);
        EXPECT_EQ(synthesize(sub, Algo::SS, true), c);
        EXPECT_EQ(c.produce_mapping(), sub);
    }
    {
        Substitution sub("0 2 3 1");
        Circuit c = SS_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::SS), c);
        EXPECT_EQ(synthesize(sub, Algo::SS, true), c);
        EXPECT_EQ(c.produce_mapping(), sub);
    }
}
