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
    EXPECT_EQ(decimal_to_binary_s(0), "0");
    EXPECT_EQ(decimal_to_binary_s(1), "1");
    EXPECT_EQ(decimal_to_binary_s(2), "10");
    EXPECT_EQ(decimal_to_binary_s(19), "10011");
    EXPECT_EQ(decimal_to_binary_s(14251525), "110110010111011000000101");

    EXPECT_EQ(decimal_to_binary_s(0, 20), "00000000000000000000");
    EXPECT_EQ(decimal_to_binary_s(1, 20), "00000000000000000001");
    EXPECT_EQ(decimal_to_binary_s(2, 20), "00000000000000000010");
    EXPECT_EQ(decimal_to_binary_s(19, 20), "00000000000000010011");
    EXPECT_EQ(decimal_to_binary_s(14251525, 20), "110110010111011000000101");

    EXPECT_EQ(decimal_to_binary_s<int>(0), "0");
    EXPECT_EQ(decimal_to_binary_s<int>(1), "1");
    EXPECT_EQ(decimal_to_binary_s<int>(2), "10");
    EXPECT_EQ(decimal_to_binary_s<int>(19), "10011");
    EXPECT_EQ(decimal_to_binary_s<int>(14251525), "110110010111011000000101");

    EXPECT_EQ(decimal_to_binary_v(0), (std::vector<bool>{0}));
    EXPECT_EQ(decimal_to_binary_v(1), (std::vector<bool>{1}));
    EXPECT_EQ(decimal_to_binary_v(2), (std::vector<bool>{1, 0}));
    EXPECT_EQ(decimal_to_binary_v(19), (std::vector<bool>{1, 0, 0, 1, 1}));
    EXPECT_EQ(decimal_to_binary_v(14251525),
              (std::vector<bool>{1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1}));

    EXPECT_EQ(decimal_to_binary_v(0, 20), (std::vector<bool>(20, false)));
    EXPECT_EQ(decimal_to_binary_v(1, 20),
              (std::vector<bool>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}));
    EXPECT_EQ(decimal_to_binary_v(2, 20),
              (std::vector<bool>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}));
    EXPECT_EQ(decimal_to_binary_v(19, 20),
              (std::vector<bool>{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1}));
    EXPECT_EQ(decimal_to_binary_v(14251525, 20),
              (std::vector<bool>{1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1}));

    EXPECT_EQ(decimal_to_binary_v<int>(0), (std::vector<bool>{0}));
    EXPECT_EQ(decimal_to_binary_v<int>(1), (std::vector<bool>{1}));
    EXPECT_EQ(decimal_to_binary_v<int>(2), (std::vector<bool>{1, 0}));
    EXPECT_EQ(decimal_to_binary_v<int>(19), (std::vector<bool>{1, 0, 0, 1, 1}));
    EXPECT_EQ(decimal_to_binary_v<int>(14251525),
              (std::vector<bool>{1, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1}));
}

TEST(Math, BinaryToDecimal) {
    EXPECT_THROW(binary_to_decimal("0010 1101"), MathException);
    EXPECT_THROW(binary_to_decimal("12"), MathException);
    EXPECT_THROW(binary_to_decimal("0010.1101"), MathException);
    EXPECT_THROW(binary_to_decimal("001021101"), MathException);

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

TEST(Math, Bits) {
    EXPECT_EQ(bits_mask(0, 3), (std::vector<size_t>{}));
    EXPECT_EQ(bits_mask(5, 3), (std::vector<size_t>{0, 2}));
    EXPECT_EQ(bits_mask(5, 5), (std::vector<size_t>{2, 4}));
    EXPECT_EQ(bits_mask(15, 4), (std::vector<size_t>{0, 1, 2, 3}));
    EXPECT_EQ(bits_mask(15, 10), (std::vector<size_t>{6, 7, 8, 9}));
}
