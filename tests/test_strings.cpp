#include <gtest/gtest.h>

#include "strings.hpp"


TEST(Strings, Trim) {
    std::string s = "";
    trim(s);
    EXPECT_EQ(s, "");

    s = " ";
    trim(s);
    EXPECT_EQ(s, "");

    s = "\t\n    \t";
    trim(s);
    EXPECT_EQ(s, "");

    s = "\t text \t\nword";
    trim(s);
    EXPECT_EQ(s, "text \t\nword");

    s = "\t text \t\nword             \n\n        \t\t";
    trim(s);
    EXPECT_EQ(s, "text \t\nword");
}

TEST(Strings, StrictStoi) {
    EXPECT_THROW(strict_stoi("4,"), StringException);
    EXPECT_THROW(strict_stoi("4."), StringException);
    EXPECT_THROW(strict_stoi("4,5"), StringException);
    EXPECT_THROW(strict_stoi("4 ff"), StringException);
    EXPECT_THROW(strict_stoi("4 23"), StringException);
    EXPECT_THROW(strict_stoi("A"), StringException);
    EXPECT_THROW(strict_stoi("0x5"), StringException);
    EXPECT_THROW(strict_stoi(""), StringException);

    EXPECT_EQ(strict_stoi("   2      \t "), 2);
    EXPECT_EQ(strict_stoi("   2      \t ", 16), std::stoi("   2      \t ", nullptr, 16));
    EXPECT_EQ(strict_stoi("Acb", 16), std::stoi("Acb", nullptr, 16));
    EXPECT_EQ(strict_stoi("Gek", 25), std::stoi("Gek", nullptr, 25));
}

TEST(Strings, StringToDecimal) {
    size_t num = 1;
    EXPECT_FALSE(try_string_to_decimal("", num));
    EXPECT_EQ(num, 1);
    EXPECT_FALSE(try_string_to_decimal("G", num));
    EXPECT_EQ(num, 1);
    EXPECT_FALSE(try_string_to_decimal("10 e", num));
    EXPECT_EQ(num, 1);
    EXPECT_FALSE(try_string_to_decimal("77  ,", num));
    EXPECT_EQ(num, 1);

    EXPECT_TRUE(try_string_to_decimal("0x154", num));
    EXPECT_EQ(num, 340);
    EXPECT_TRUE(try_string_to_decimal("0X154", num));
    EXPECT_EQ(num, 340);
    EXPECT_TRUE(try_string_to_decimal("0xAb3c", num));
    EXPECT_EQ(num, 43836);
    EXPECT_TRUE(try_string_to_decimal("0XAb3c", num));
    EXPECT_EQ(num, 43836);

    EXPECT_TRUE(try_string_to_decimal("A", num));
    EXPECT_EQ(num, 10);
    EXPECT_TRUE(try_string_to_decimal("def", num));
    EXPECT_EQ(num, 3567);

    EXPECT_TRUE(try_string_to_decimal("12", num));
    EXPECT_EQ(num, 12);
    EXPECT_TRUE(try_string_to_decimal("2141212", num));
    EXPECT_EQ(num, 2141212);
}

TEST(Strings, StringToVector) {
    EXPECT_THROW(string_to_num_vector("0 3    2 r 32"), StringException);
    EXPECT_THROW(string_to_num_vector("0, 3  ,  2, 32"), StringException);
    EXPECT_THROW(string_to_num_vector("1, 2, 3, 4; 5, 2", ','), StringException);
    EXPECT_THROW(string_to_num_vector(","), StringException);

    EXPECT_EQ(string_to_num_vector(""), std::vector<size_t>());
    EXPECT_EQ(string_to_num_vector(",", ','), std::vector<size_t>());
    EXPECT_EQ(string_to_num_vector<int>("\t      \t 5 \t 0x2 4       A"), std::vector<int>({5, 2, 4, 10}));
    EXPECT_EQ(string_to_num_vector<int>("\t      \t 5; \t 0x2; 4    ;   A", ';'), std::vector<int>({5, 2, 4, 10}));
}

TEST(Strings, ToLower) {
    std::string s;
    to_lower(s);
    EXPECT_EQ(s, "");

    s = "text";
    to_lower(s);
    EXPECT_EQ(s, "text");

    s = "TExt";
    to_lower(s);
    EXPECT_EQ(s, "text");

    s = "1245.,;";
    to_lower(s);
    EXPECT_EQ(s, "1245.,;");
}
