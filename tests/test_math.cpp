#include <gtest/gtest.h>

#include "math.hpp"

TEST(Math, IsPowerOf2) {
    EXPECT_TRUE(is_power_of_2(1));
    EXPECT_TRUE(is_power_of_2(2));
    EXPECT_TRUE(is_power_of_2(4));
    EXPECT_TRUE(is_power_of_2(1125899906842624));

    EXPECT_FALSE(is_power_of_2(0));
    EXPECT_FALSE(is_power_of_2(3));
    EXPECT_FALSE(is_power_of_2(5));
    EXPECT_FALSE(is_power_of_2(198765435167481243));

    EXPECT_TRUE(is_power_of_2<int>(16));
    EXPECT_FALSE(is_power_of_2<int>(-2));
    EXPECT_TRUE(is_power_of_2<long long int>(2251799813685248));
}


TEST(Math, DecimalToBinary) {
    EXPECT_EQ(decimal_to_binary(0), "0");
    EXPECT_EQ(decimal_to_binary(1), "1");
    EXPECT_EQ(decimal_to_binary(2), "10");
    EXPECT_EQ(decimal_to_binary(19), "10011");
    EXPECT_EQ(decimal_to_binary(14251525), "110110010111011000000101");

    EXPECT_EQ(decimal_to_binary(0, 20), "00000000000000000000");
    EXPECT_EQ(decimal_to_binary(1, 20), "00000000000000000001");
    EXPECT_EQ(decimal_to_binary(2, 20), "00000000000000000010");
    EXPECT_EQ(decimal_to_binary(19, 20), "00000000000000010011");
    EXPECT_EQ(decimal_to_binary(14251525, 20), "110110010111011000000101");

    EXPECT_EQ(decimal_to_binary<int>(0), "0");
    EXPECT_EQ(decimal_to_binary<int>(1), "1");
    EXPECT_EQ(decimal_to_binary<int>(2), "10");
    EXPECT_EQ(decimal_to_binary<int>(19), "10011");
    EXPECT_EQ(decimal_to_binary<int>(14251525), "110110010111011000000101");
}

TEST(Math, BinaryToDecimal) {
    EXPECT_THROW(binary_to_decimal("0010 1101"), std::runtime_error);
    EXPECT_THROW(binary_to_decimal("12"), std::runtime_error);
    EXPECT_THROW(binary_to_decimal("0010.1101"), std::runtime_error);
    EXPECT_THROW(binary_to_decimal("001021101"), std::runtime_error);

    EXPECT_EQ(binary_to_decimal("0"), 0);
    EXPECT_EQ(binary_to_decimal("1"), 1);
    EXPECT_EQ(binary_to_decimal("10"), 2);
    EXPECT_EQ(binary_to_decimal("10011"), 19);
    EXPECT_EQ(binary_to_decimal("110110010111011000000101"), 14251525);

    EXPECT_EQ(binary_to_decimal("00000000000000000000"), 0);
    EXPECT_EQ(binary_to_decimal("00000000000000000001"), 1);
    EXPECT_EQ(binary_to_decimal("00000000000000000010"), 2);
    EXPECT_EQ(binary_to_decimal("00000000000000010011"), 19);
    EXPECT_EQ(binary_to_decimal("110110010111011000000101"), 14251525);

    EXPECT_EQ(binary_to_decimal<int>("0"), 0);
    EXPECT_EQ(binary_to_decimal<int>("1"), 1);
    EXPECT_EQ(binary_to_decimal<int>("10"), 2);
    EXPECT_EQ(binary_to_decimal<int>("10011"), 19);
    EXPECT_EQ(binary_to_decimal<int>("110110010111011000000101"), 14251525);
}

TEST(Math, BinaryDot) {
    EXPECT_EQ(binary_dot(0, 1), 0);         // 0 * 1
    EXPECT_EQ(binary_dot(10, 1), 0);        // 1010 * 0001
    EXPECT_EQ(binary_dot(7, 3), 2);         // 111 * 11
    EXPECT_EQ(binary_dot(1023, 2047), 10);  // 1111111111 * 11111111111
}
