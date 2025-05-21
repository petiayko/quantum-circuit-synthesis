#include <gtest/gtest.h>

#include "primitives.hpp"


TEST(Substitutions, Constructor) {
    EXPECT_THROW(Substitution(cf_set({})), SubException);
    EXPECT_THROW(Substitution(cf_set({BooleanFunction("1010"), BooleanFunction("10")})), SubException);
    EXPECT_THROW(Substitution(cf_set({BooleanFunction("1010"), BooleanFunction("1001"), BooleanFunction("1010")})),
                 SubException);
    EXPECT_THROW(Substitution(cf_set({BooleanFunction("1010"), BooleanFunction("1010")})), SubException);

    EXPECT_THROW(Substitution(table({})), SubException);
    EXPECT_THROW(Substitution(table({{}})), SubException);
    EXPECT_THROW(Substitution(table({{0},
                                     {1}})), SubException);
    EXPECT_THROW(Substitution(table({{1, 0, 1, 0},
                                     {1, 0}})), SubException);
    EXPECT_THROW(Substitution(table({{1, 0, 1, 0},
                                     {1, 0, 0, 1},
                                     {1, 0, 0, 1}})), SubException);
    EXPECT_THROW(Substitution(table({{1, 0, 1, 0},
                                     {1, 0, 0, 0}})), SubException);

    EXPECT_THROW(Substitution(""), SubException);
    EXPECT_THROW(Substitution("1"), SubException);
    EXPECT_THROW(Substitution("0 1 3"), SubException);
    EXPECT_THROW(Substitution("0 1 1 2"), SubException);
    EXPECT_THROW(Substitution("0 1 2 r 3"), SubException);
    EXPECT_THROW(Substitution("0 1 2 3 4 5 6 7 8 9 A B C D E F G"), SubException);
    EXPECT_THROW(Substitution("# 0 1 2 3 4 5 6 7 8 9 A B C D E F"), SubException);

    EXPECT_NO_THROW(Substitution("0"));
    EXPECT_NO_THROW(Substitution("3           2 1 0"));
    EXPECT_NO_THROW(Substitution("3\t2\t1\t0"));
    EXPECT_NO_THROW(Substitution("0 1 2 3 4 5 6 7 8 9 A B C D E F"));
    EXPECT_NO_THROW(Substitution("0 1 2 3 4 5 6 7 8 9 a b c d e f"));
    EXPECT_NO_THROW(Substitution("0 1 2 3 4 5 6 7 8 9 a B c D E f"));
    EXPECT_NO_THROW(Substitution("0 1 2 3 4 5 6 7 8 9\n a B c D E f"));
    EXPECT_NO_THROW(Substitution("0 1 2 3 4 5 6 7 8 9\n# comment\n a B c D E f"));
    EXPECT_NO_THROW(Substitution("0x0 2 0X3 0x4 0X5 0x1 0x7 0x6 0x8 0x9 C 0xb 0xA"));

    Substitution s("0 4 1 2 3 5");
    Substitution s1(s);
    Substitution s2 = s1;
    EXPECT_EQ(s, s1);
    EXPECT_EQ(s, s2);
    EXPECT_EQ(s, Substitution("0 4 1 2 3 5"));

    auto s_vec = Substitution("0 2 1 3").vector();
    EXPECT_EQ(s_vec[0], 0);
    EXPECT_EQ(s_vec[1], 2);
    EXPECT_EQ(s_vec[2], 1);
    EXPECT_EQ(s_vec[3], 3);
}

TEST(Substitutions, Methods) {
    Substitution s("0 2 4 1 3");
    EXPECT_EQ(s.power(), 5);

    auto cycles = s.cycles();
    auto transpositions = s.transpositions();
    ASSERT_EQ(cycles.size(), 2);
    EXPECT_EQ(cycles[0], (std::vector<size_t>{0}));
    EXPECT_EQ(cycles[1], (std::vector<size_t>{1, 2, 4, 3}));
    EXPECT_TRUE(s.is_odd());
    EXPECT_EQ(transpositions, (std::vector<std::pair<size_t, size_t>>{{1, 2},
                                                                      {2, 4},
                                                                      {4, 3}}));

    Substitution s1("0 1 2 3");
    cycles = s1.cycles();
    transpositions = s1.transpositions();
    ASSERT_EQ(cycles.size(), 4);
    EXPECT_EQ(cycles[0], (std::vector<size_t>{0}));
    EXPECT_EQ(cycles[1], (std::vector<size_t>{1}));
    EXPECT_EQ(cycles[2], (std::vector<size_t>{2}));
    EXPECT_EQ(cycles[3], (std::vector<size_t>{3}));
    EXPECT_FALSE(s1.is_odd());
    EXPECT_EQ(transpositions, (std::vector<std::pair<size_t, size_t>>{}));
    EXPECT_TRUE(s1.is_identical());

    Substitution s2("1 0 3 2 5 4 7 6");
    cycles = s2.cycles();
    transpositions = s2.transpositions();
    ASSERT_EQ(cycles.size(), 4);
    EXPECT_EQ(cycles[0], (std::vector<size_t>{0, 1}));
    EXPECT_EQ(cycles[1], (std::vector<size_t>{2, 3}));
    EXPECT_EQ(cycles[2], (std::vector<size_t>{4, 5}));
    EXPECT_EQ(cycles[3], (std::vector<size_t>{6, 7}));
    EXPECT_FALSE(s2.is_odd());
    EXPECT_EQ(transpositions, (std::vector<std::pair<size_t, size_t>>{{0, 1},
                                                                      {2, 3},
                                                                      {4, 5},
                                                                      {6, 7}}));
    EXPECT_FALSE(s2.is_identical());

    Substitution s3("0");
    cycles = s3.cycles();
    transpositions = s3.transpositions();
    ASSERT_EQ(cycles.size(), 1);
    EXPECT_EQ(cycles[0], (std::vector<size_t>{0}));
    EXPECT_FALSE(s3.is_odd());
    EXPECT_EQ(transpositions, (std::vector<std::pair<size_t, size_t>>{}));
    EXPECT_TRUE(s3.is_identical());
}

TEST(Substitutions, Stream) {
    auto s = Substitution("0x0 2 0X3 0x4 0X5 0x1 0x7 0x6 0x8 0x9 C 0xb 0xA");
    std::stringstream out_stream;

    out_stream << s;
    EXPECT_EQ(out_stream.str(), "0 2 3 4 5 1 7 6 8 9 12 11 10 ");
    EXPECT_NO_THROW(Substitution(out_stream.str()));

    out_stream.str("");
    s = Substitution("0");
    out_stream << s;
    EXPECT_EQ(out_stream.str(), "0 ");
    EXPECT_NO_THROW(Substitution(out_stream.str()));

    std::ifstream file("../tests/assets/sub.txt", std::ios::in);
    s = Substitution(file);
    EXPECT_EQ(s, Substitution("11 0 1 2 5 14 4 10 3 15 6 7 13 8 12 9"));

    std::ifstream file1("../tests/assets/map.txt", std::ios::in);
    EXPECT_THROW((Substitution(file1)), SubException);

    std::ifstream file2("../tests/assets/qc.txt", std::ios::in);
    EXPECT_THROW((Substitution(file2)), SubException);
}

TEST(Substitutions, BinaryMappings) {
    auto mp = BinaryMapping(cf_set({BooleanFunction("1010"), BooleanFunction("1001")}));
    auto s = Substitution(mp);
    EXPECT_EQ(s, Substitution("3 0 2 1"));

    mp = BinaryMapping(cf_set({BooleanFunction("1001"), BooleanFunction("1010")}));
    s = Substitution(mp);
    EXPECT_EQ(s, Substitution("3 0 1 2"));

    mp = BinaryMapping(cf_set({BooleanFunction("01110010"), BooleanFunction("11100100"), BooleanFunction("10110001")}));
    s = mp;
    EXPECT_EQ(s, Substitution("3 6 7 5 0 2 4 1"));

    auto mp_cursed_1 = BinaryMapping(cf_set({BooleanFunction("1001"), BooleanFunction("1001")}));
    auto mp_cursed_2 = BinaryMapping(cf_set({BooleanFunction("1101"), BooleanFunction("0011")}));
    auto mp_cursed_3 = BinaryMapping(
            cf_set({BooleanFunction("01110010"), BooleanFunction("11100100"), BooleanFunction("10110001"),
                    BooleanFunction("01010101")}));
    EXPECT_THROW((Substitution(mp_cursed_1)), SubException);
    EXPECT_THROW((Substitution(mp_cursed_2)), SubException);
    EXPECT_THROW((Substitution(mp_cursed_3)), SubException);
    EXPECT_THROW((s = mp_cursed_1), SubException);
    EXPECT_THROW((s = mp_cursed_2), SubException);
    EXPECT_THROW((s = mp_cursed_3), SubException);
}
