#include <gtest/gtest.h>

#include "primitives.hpp"

TEST(BooleanFunction, Constructor) {
    EXPECT_THROW(BooleanFunction(static_cast<size_t>(2), 0), std::runtime_error);
    EXPECT_THROW(BooleanFunction(static_cast<size_t>(2), 1), std::runtime_error);
    EXPECT_THROW(BooleanFunction(static_cast<size_t>(2), 2), std::runtime_error);
    EXPECT_THROW(BooleanFunction(true, 0), std::runtime_error);
    EXPECT_THROW(BooleanFunction(false, 0), std::runtime_error);
    EXPECT_THROW(BooleanFunction(binary_vector{}), std::runtime_error);
    EXPECT_THROW(BooleanFunction(binary_vector{0}), std::runtime_error);
    EXPECT_THROW(BooleanFunction(binary_vector{0, 0, 0, 1, 1}), std::runtime_error);
    EXPECT_THROW(BooleanFunction(std::vector<int>{1}), std::runtime_error);
    EXPECT_THROW(BooleanFunction(std::vector<int>{0, 0, 0, 1, 1}), std::runtime_error);
    EXPECT_THROW(BooleanFunction(std::vector<int>{0, 0, 2, 1, 1}), std::runtime_error);
    EXPECT_THROW(BooleanFunction(std::vector<int>{3}), std::runtime_error);
    EXPECT_THROW(BooleanFunction(""), std::runtime_error);
    EXPECT_THROW(BooleanFunction("1"), std::runtime_error);
    EXPECT_THROW(BooleanFunction("011"), std::runtime_error);
    EXPECT_THROW(BooleanFunction("021"), std::runtime_error);

    EXPECT_NO_THROW(BooleanFunction());
    EXPECT_EQ(BooleanFunction(static_cast<size_t>(0), 1), BooleanFunction("01"));
    EXPECT_EQ(BooleanFunction(static_cast<size_t>(0), 2), BooleanFunction("0011"));
    EXPECT_EQ(BooleanFunction(static_cast<size_t>(1), 2), BooleanFunction("0101"));
    EXPECT_EQ(BooleanFunction(static_cast<size_t>(0), 4), BooleanFunction("0000000011111111"));
    EXPECT_EQ(BooleanFunction(static_cast<size_t>(2), 4), BooleanFunction("0011001100110011"));
    EXPECT_EQ(BooleanFunction(false, 1), BooleanFunction("00"));
    EXPECT_EQ(BooleanFunction(true, 3), BooleanFunction("11111111"));
    EXPECT_EQ(BooleanFunction(false, 4), BooleanFunction("0000000000000000"));

    BooleanFunction bf_s("10001010");
    BooleanFunction bf_s_copy_1(bf_s);
    BooleanFunction bf_v_bool(binary_vector{1, 0, 0, 0, 1, 0, 1, 0});
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

    BooleanFunction bf_2("01");
    EXPECT_EQ(bf_2.size(), 2);
    EXPECT_EQ(bf_2.dim(), 1);
    EXPECT_EQ(bf_2.weight(), 1);
    EXPECT_EQ(bf_2.is_balanced(), true);

    EXPECT_EQ(bf_1.vector(), binary_vector({true, false, false, false, true, false, true, false}));
    EXPECT_EQ(bf_2.vector(), binary_vector({false, true}));
    EXPECT_EQ(BooleanFunction().vector(), binary_vector());

    EXPECT_EQ(BooleanFunction("01").extend(), BinaryMapping({BooleanFunction("01")}));
    EXPECT_TRUE(BooleanFunction("01").extend().is_substitution());
    EXPECT_EQ(BooleanFunction("00").extend(), BinaryMapping({BooleanFunction("0011"), BooleanFunction("0101")}));
    EXPECT_TRUE(BooleanFunction("00").extend().is_substitution());
    EXPECT_EQ(BooleanFunction("1111").extend(),
              BinaryMapping({BooleanFunction("00001111"), BooleanFunction("00110011"), BooleanFunction("10101010")}));
    EXPECT_TRUE(BooleanFunction("1111").extend().is_substitution());
    EXPECT_EQ(BooleanFunction("1001").extend(), BinaryMapping({BooleanFunction("0011"), BooleanFunction("1001")}));
    EXPECT_TRUE(BooleanFunction("1001").extend().is_substitution());
    EXPECT_EQ(BooleanFunction("1101").extend(),
              BinaryMapping({BooleanFunction("00001111"), BooleanFunction("00110011"), BooleanFunction("10100110")}));
    EXPECT_TRUE(BooleanFunction("1101").extend().is_substitution());
    EXPECT_EQ(BooleanFunction("00101001").extend(), BinaryMapping(
            {
                BooleanFunction("0000000011111111"), BooleanFunction("0000111100001111"),
                        BooleanFunction("0011001100110011"), BooleanFunction("0101100110010110")
            }));
    EXPECT_TRUE(BooleanFunction("00101001").extend().is_substitution());

    EXPECT_EQ(BooleanFunction("01").RW_spectrum(), (std::vector<int>{1, -1}));
    EXPECT_EQ(BooleanFunction("00").RW_spectrum(), (std::vector<int>{0, 0}));
    EXPECT_EQ(BooleanFunction("1011").RW_spectrum(), (std::vector<int>{3, 1, -1, 1}));
    EXPECT_EQ(BooleanFunction("11000100").RW_spectrum(), (std::vector<int>{3, -1, 3, -1, 1, 1, 1, 1}));
    EXPECT_EQ(BooleanFunction("00000101").RW_spectrum(), (std::vector<int>{2, -2, 0, 0, -2, 2, 0, 0}));
    EXPECT_EQ(BooleanFunction("1000111000010110").RW_spectrum(),
              (std::vector<int>{7, 1, 1, -1, -3, -1, -1, 5, 1, 3, 3, 1, -1, 1, 1, -1}));

    EXPECT_EQ(BooleanFunction("01").adjacent_zeros(), 0);
    EXPECT_EQ(BooleanFunction("00").adjacent_zeros(), 1);
    EXPECT_EQ(BooleanFunction("1011").adjacent_zeros(), 2);
    EXPECT_EQ(BooleanFunction("11000100").adjacent_zeros(), 7);
    EXPECT_EQ(BooleanFunction("00000101").adjacent_zeros(), 8);
    EXPECT_EQ(BooleanFunction("1000111000010110").adjacent_zeros(), 14);
    EXPECT_EQ(BooleanFunction("11111111111111111111111111111111").adjacent_zeros(), 80);
    EXPECT_EQ(BooleanFunction("00000000000000000000000000000000").adjacent_zeros(), 80);

    EXPECT_EQ(BooleanFunction("01").complexity(), 0);
    EXPECT_EQ(BooleanFunction("00").complexity(), 5);
    EXPECT_EQ(BooleanFunction("1011").complexity(), 2);
    EXPECT_EQ(BooleanFunction("11000100").complexity(), 7);
    EXPECT_EQ(BooleanFunction("00000101").complexity(), 104);
    EXPECT_EQ(BooleanFunction("1000111000010110").complexity(), 14);
}

TEST(BooleanFunction, Operators) {
    EXPECT_THROW(BooleanFunction("1000") + BooleanFunction("10"), std::runtime_error);
    EXPECT_THROW(BooleanFunction("0011") + BooleanFunction("11011001"), std::runtime_error);
    EXPECT_THROW(BooleanFunction("1000") * BooleanFunction("10"), std::runtime_error);
    EXPECT_THROW(BooleanFunction("0011") * BooleanFunction("11011001"), std::runtime_error);
    EXPECT_THROW(BooleanFunction("1000") | BooleanFunction("10"), std::runtime_error);
    EXPECT_THROW(BooleanFunction("0011") | BooleanFunction("11011001"), std::runtime_error);

    EXPECT_EQ(BooleanFunction("0011") + BooleanFunction("1101"), BooleanFunction("1110"));
    EXPECT_EQ(BooleanFunction("1101") + BooleanFunction("0011"), BooleanFunction("1110"));
    EXPECT_EQ(BooleanFunction("11001100") + BooleanFunction("00110011"), BooleanFunction("11111111"));
    EXPECT_EQ(BooleanFunction("00110011") + BooleanFunction("11001100"), BooleanFunction("11111111"));
    EXPECT_EQ(BooleanFunction("0011") * BooleanFunction("1101"), BooleanFunction("0001"));
    EXPECT_EQ(BooleanFunction("1101") * BooleanFunction("0011"), BooleanFunction("0001"));
    EXPECT_EQ(BooleanFunction("11001100") * BooleanFunction("00110011"), BooleanFunction("00000000"));
    EXPECT_EQ(BooleanFunction("00110011") * BooleanFunction("11001100"), BooleanFunction("00000000"));
    EXPECT_EQ(BooleanFunction("0011") | BooleanFunction("1101"), BooleanFunction("1111"));
    EXPECT_EQ(BooleanFunction("1101") | BooleanFunction("0011"), BooleanFunction("1111"));
    EXPECT_EQ(BooleanFunction("10000100") | BooleanFunction("11001001"), BooleanFunction("11001101"));
    EXPECT_EQ(BooleanFunction("11001001") | BooleanFunction("10000100"), BooleanFunction("11001101"));
    EXPECT_EQ(~BooleanFunction("1101"), BooleanFunction("0010"));
    EXPECT_EQ(~BooleanFunction("10000100"), BooleanFunction("01111011"));
    EXPECT_EQ((BooleanFunction("0100") + BooleanFunction("1101") * ~BooleanFunction("1100")) | BooleanFunction("1000"),
              BooleanFunction("1101"));

    BooleanFunction bf1("11000010");
    BooleanFunction bf2("01010100");
    bf1 += BooleanFunction("11110000");
    bf2 += bf1;
    EXPECT_EQ(bf2, BooleanFunction("01100110"));

    BooleanFunction bf3("11000010");
    BooleanFunction bf4("01010100");
    bf3 *= BooleanFunction("11110000");
    bf4 *= bf3;
    EXPECT_EQ(bf4, BooleanFunction("01000000"));

    BooleanFunction bf5("11000010");
    BooleanFunction bf6("01010100");
    bf5 |= BooleanFunction("11110000");
    bf6 |= bf5;
    EXPECT_EQ(bf6, BooleanFunction("11110110"));

    EXPECT_EQ(~bf6, BooleanFunction("00001001"));
    EXPECT_EQ(~bf6, BooleanFunction("11110110"));
}

TEST(BooleanFunction, Stream) {
    BooleanFunction bf_1("10001010");
    std::stringstream out_stream;

    out_stream << bf_1;
    EXPECT_EQ(out_stream.str(), "10001010");
    EXPECT_NO_THROW(BooleanFunction(out_stream.str()));

    EXPECT_EQ(bf_1.to_table(), "000\t1\n001\t0\n010\t0\n011\t0\n100\t1\n101\t0\n110\t1\n111\t0\n");
    EXPECT_EQ(bf_1.to_table('_'), "000_1\n001_0\n010_0\n011_0\n100_1\n101_0\n110_1\n111_0\n");
}
