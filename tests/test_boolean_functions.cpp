#include <gtest/gtest.h>

#include "primitives.hpp"

TEST(BooleanFunction, Constructor) {
    EXPECT_THROW(BooleanFunction(std::vector<bool>{}), std::runtime_error);
    EXPECT_THROW(BooleanFunction(std::vector<bool>{0, 0, 0, 1, 1}), std::runtime_error);
    EXPECT_THROW(BooleanFunction(std::vector<int>{0, 0, 0, 1, 1}), std::runtime_error);
    EXPECT_THROW(BooleanFunction(std::vector<int>{0, 0, 2, 1, 1}), std::runtime_error);
    EXPECT_THROW(BooleanFunction(std::vector<int>{3}), std::runtime_error);
    EXPECT_THROW(BooleanFunction(""), std::runtime_error);
    EXPECT_THROW(BooleanFunction("011"), std::runtime_error);
    EXPECT_THROW(BooleanFunction("021"), std::runtime_error);

    BooleanFunction bf_s("10001010");
    BooleanFunction bf_s_copy_1(bf_s);
    BooleanFunction bf_v_bool(std::vector<bool>{1, 0, 0, 0, 1, 0, 1, 0});
    BooleanFunction bf_v_int(std::vector<int>{1, 0, 0, 0, 1, 0, 1, 0});
    EXPECT_EQ(bf_s, bf_v_bool);
    EXPECT_EQ(bf_s, bf_v_int);
    EXPECT_EQ(bf_s, bf_s_copy_1);

    BooleanFunction bf_s_uneq_1("1010");
    BooleanFunction bf_s_uneq_2("10101111");
    EXPECT_NE(bf_s, bf_s_uneq_1);
    EXPECT_NE(bf_s, bf_s_uneq_2);
    EXPECT_NE(bf_s_uneq_1, bf_s_uneq_2);

    BooleanFunction bf_s_copy_2 = bf_s_uneq_1;
    EXPECT_EQ(bf_s_copy_2, bf_s_uneq_1);
}


TEST(BooleanFunction, Methods) {
    BooleanFunction bf_1("10001010");
    EXPECT_EQ(bf_1.size(), 8);
    EXPECT_EQ(bf_1.dim(), 3);
    EXPECT_EQ(bf_1.weight(), 3);
    EXPECT_EQ(bf_1.is_balanced(), false);

    BooleanFunction bf_2("0");
    EXPECT_EQ(bf_2.size(), 1);
    EXPECT_EQ(bf_2.dim(), 0);
    EXPECT_EQ(bf_2.weight(), 0);
    EXPECT_EQ(bf_2.is_balanced(), false);

    BooleanFunction bf_3("01");
    EXPECT_EQ(bf_3.size(), 2);
    EXPECT_EQ(bf_3.dim(), 1);
    EXPECT_EQ(bf_3.weight(), 1);
    EXPECT_EQ(bf_3.is_balanced(), true);
}

TEST(BooleanFunction, Stream) {
    BooleanFunction bf_1("10001010");
    std::stringstream out_stream;

    out_stream << bf_1;
    EXPECT_EQ(out_stream.str(), "10001010");

    EXPECT_EQ(bf_1.to_table(), "000\t1\n001\t0\n010\t0\n011\t0\n100\t1\n101\t0\n110\t1\n111\t0\n");
    EXPECT_EQ(bf_1.to_table('_'), "000_1\n001_0\n010_0\n011_0\n100_1\n101_0\n110_1\n111_0\n");
}
