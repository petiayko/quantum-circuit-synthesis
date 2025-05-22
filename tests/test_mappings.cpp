#include <gtest/gtest.h>

#include "primitives.hpp"


TEST(BinaryMappings, Constructor) {
    EXPECT_THROW(BinaryMapping(cf_set({})), BMException);
    EXPECT_THROW(BinaryMapping(cf_set({BooleanFunction("1001"),
                                       BooleanFunction("10")})),
                 BMException);
    EXPECT_THROW(BinaryMapping(cf_set({BooleanFunction("0101"),
                                       BooleanFunction("1000"),
                                       BooleanFunction("01")})),
                 BMException);

    EXPECT_THROW(BinaryMapping(table({})), BMException);
    EXPECT_THROW(BinaryMapping(table({{1, 0, 0, 1},
                                      {0, 1, 1, 1},
                                      {1, 0}})), BMException);

    EXPECT_THROW(BinaryMapping(""), BMException);
    EXPECT_THROW(BinaryMapping("001\n110\n1 0"), BMException);
    EXPECT_THROW(BinaryMapping("001\n110\n101\n01"), BMException);
    EXPECT_THROW(BinaryMapping("001\n110\n121\n011"), BMException);
    EXPECT_THROW(BinaryMapping("#001\n110\n101\n011"), BFException);

    EXPECT_NO_THROW(BinaryMapping("001\n110\n\n101\n011"));
    EXPECT_NO_THROW(BinaryMapping("0 0 1\n1 1 0\n1 0 1\n0 0 0"));
    EXPECT_NO_THROW(BinaryMapping("0 0 1\n#comment\n\n\n\n1 1 0\n1 0 1\n0 0 0"));
    EXPECT_NO_THROW(BinaryMapping("0 0                   1\n110\n              1 0 1\n0 00"));

    BinaryMapping m1("01\n10\n11\n00");
    BinaryMapping m2(m1);
    BinaryMapping m3 = m2;
    EXPECT_EQ(m1, m2);
    EXPECT_EQ(m2, m3);
    EXPECT_NE(m1, BinaryMapping("0\n1"));

    auto vec1 = BinaryMapping("101\n111\n001\n110\n100\n111\n101\n000").coordinate_functions();
    EXPECT_EQ(vec1[0], BooleanFunction("11011110"));
    EXPECT_EQ(vec1[1], BooleanFunction("01010100"));
    EXPECT_EQ(vec1[2], BooleanFunction("11100110"));

    auto vec2 = BinaryMapping(table({
                                            {1, 1, 0, 1},
                                            {0, 0, 1, 0},
                                            {0, 1, 0, 0},
                                            {1, 0, 1, 0},
                                            {0, 1, 1, 1},
                                            {1, 0, 0, 0},
                                            {0, 1, 0, 1},
                                            {1, 0, 0, 1},
                                    })).coordinate_functions();
    EXPECT_EQ(vec2[0], BooleanFunction("1101"));
    EXPECT_EQ(vec2[1], BooleanFunction("0010"));
    EXPECT_EQ(vec2[2], BooleanFunction("0100"));
    EXPECT_EQ(vec2[3], BooleanFunction("1010"));
    EXPECT_EQ(vec2[4], BooleanFunction("0111"));
    EXPECT_EQ(vec2[5], BooleanFunction("1000"));
    EXPECT_EQ(vec2[6], BooleanFunction("0101"));
    EXPECT_EQ(vec2[7], BooleanFunction("1001"));

    auto vec3 = BinaryMapping(table({
                                            {1, 1, 0, 1, 0, 0, 1, 0},
                                            {0, 1, 1, 1, 1, 0, 0, 0},
                                    })).coordinate_functions();
    EXPECT_EQ(vec3[0], BooleanFunction("11010010"));
    EXPECT_EQ(vec3[1], BooleanFunction("01111000"));

    auto vec4 = BinaryMapping(cf_set({
                                             BooleanFunction("01"),
                                             BooleanFunction("10"),
                                             BooleanFunction("01")
                                     })).coordinate_functions();
    EXPECT_EQ(vec4[0], BooleanFunction("01"));
    EXPECT_EQ(vec4[1], BooleanFunction("10"));
    EXPECT_EQ(vec4[2], BooleanFunction("01"));
}

TEST(BinaryMappings, Methods) {
    auto bm = BinaryMapping(table({
                                          {1, 1, 0, 1},
                                          {0, 0, 1, 0},
                                          {0, 1, 0, 0},
                                          {1, 0, 1, 0},
                                          {0, 1, 1, 1},
                                          {1, 0, 0, 0},
                                          {0, 1, 0, 1},
                                          {1, 0, 0, 1},
                                  }));
    EXPECT_EQ(bm.inputs_number(), 2);
    EXPECT_EQ(bm.outputs_number(), 8);

    auto bm1 = BinaryMapping(table({
                                           {1, 1, 0, 1, 0, 0, 1, 0},
                                           {0, 1, 1, 1, 1, 0, 0, 0},
                                   }));
    EXPECT_EQ(bm1.inputs_number(), 3);
    EXPECT_EQ(bm1.outputs_number(), 2);

    EXPECT_EQ(BinaryMapping(table{{0, 1}}).extend(), BinaryMapping(table{{0, 1}}));
    EXPECT_TRUE(BinaryMapping(table{{0, 1}}).extend().is_substitution());

    EXPECT_EQ(BinaryMapping(table{{0, 0, 1, 1}, {0, 1, 0, 1}}).extend(), BinaryMapping(table{{0, 0, 1, 1},
                                                                                             {0, 1, 0, 1}}));
    EXPECT_TRUE(BinaryMapping(table{{0, 0, 1, 1}, {0, 1, 0, 1}}).extend().is_substitution());

    EXPECT_EQ(BinaryMapping(table{{0, 0, 0, 1}}).extend(), BinaryMapping(table{{0, 0, 0, 0, 1, 1, 1, 1},
                                                                               {0, 0, 1, 1, 0, 0, 1, 1},
                                                                               {0, 1, 0, 1, 0, 1, 1, 0}}));
    EXPECT_TRUE(BinaryMapping(table{{0, 0, 0, 1}}).extend().is_substitution());

    EXPECT_EQ(BinaryMapping(table{{1, 0},
                                  {1, 1}}).extend(), BinaryMapping(table{{0, 0, 0, 0, 1, 1, 1, 1},
                                                                         {1, 1, 0, 0, 0, 0, 1, 1},
                                                                         {1, 0, 1, 0, 1, 0, 1, 0}}));
    EXPECT_TRUE(BinaryMapping(table{{1, 0},
                                    {1, 1}}).extend().is_substitution());

    EXPECT_EQ(BinaryMapping(table{{1, 0, 1, 0},
                                  {1, 0, 1, 0}}).extend(),
              BinaryMapping(table{{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
                                  {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1},
                                  {1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1},
                                  {1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1}}));
    EXPECT_TRUE(BinaryMapping(table{{1, 0, 1, 0},
                                    {1, 0, 1, 0}}).extend().is_substitution());

    EXPECT_EQ(BinaryMapping(table{{1, 1, 1, 1},
                                  {0, 0, 0, 0}}).extend(),
              BinaryMapping(table{{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
                                  {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1},
                                  {1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
                                  {0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1}}));
    EXPECT_TRUE(BinaryMapping(table{{1, 1, 1, 1},
                                    {0, 0, 0, 0}}).extend().is_substitution());

    EXPECT_EQ(BinaryMapping(table{{1, 0},
                                  {1, 1},
                                  {0, 1}}).extend(),
              BinaryMapping(table{{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
                                  {1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
                                  {1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0},
                                  {0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0}}));
    EXPECT_TRUE(BinaryMapping(table{{1, 0},
                                    {1, 1},
                                    {0, 1}}).extend().is_substitution());

    auto bm_ext = bm.extend();
    EXPECT_TRUE(bm_ext.extend().is_substitution());
    EXPECT_EQ(bm_ext.inputs_number(), bm.inputs_number() + bm.outputs_number());
    EXPECT_EQ(bm_ext.outputs_number(), bm.inputs_number() + bm.outputs_number());

    auto bm1_ext = bm1.extend();
    EXPECT_TRUE(bm1_ext.extend().is_substitution());
    EXPECT_EQ(bm1_ext.inputs_number(), bm1.inputs_number() + bm1.outputs_number());
    EXPECT_EQ(bm1_ext.outputs_number(), bm1.inputs_number() + bm1.outputs_number());

}

TEST(BinaryMapping, Stream) {
    auto bm = BinaryMapping(table({
                                          {1, 1, 0, 1},
                                          {0, 0, 1, 0},
                                          {0, 1, 0, 0},
                                          {1, 0, 1, 0},
                                          {0, 1, 1, 1},
                                          {1, 0, 0, 0},
                                          {0, 1, 0, 1},
                                          {1, 0, 0, 1},
                                  }));
    std::stringstream out_stream;

    out_stream << bm;
    EXPECT_EQ(out_stream.str(), "10010101\n"
                                "10101010\n"
                                "01011000\n"
                                "10001011\n");
    EXPECT_EQ(bm.to_table(), "00\t10010101\n"
                             "01\t10101010\n"
                             "10\t01011000\n"
                             "11\t10001011\n");
    EXPECT_EQ(bm.to_table('a'), "00a10010101\n"
                                "01a10101010\n"
                                "10a01011000\n"
                                "11a10001011\n");

    auto bm1 = BinaryMapping(cf_set({
                                            BooleanFunction("01"),
                                            BooleanFunction("10"),
                                            BooleanFunction("01"),
                                            BooleanFunction("01"),
                                            BooleanFunction("11"),
                                            BooleanFunction("00"),
                                            BooleanFunction("00"),
                                            BooleanFunction("10"),
                                            BooleanFunction("11"),
                                            BooleanFunction("11"),
                                            BooleanFunction("11"),
                                            BooleanFunction("00"),
                                    }));
    out_stream.str("");

    out_stream << bm1;
    EXPECT_EQ(out_stream.str(), "010010011110\n"
                                "101110001110\n");
    EXPECT_EQ(bm1.to_table(), "0\t010010011110\n"
                              "1\t101110001110\n");
    EXPECT_NO_THROW(BinaryMapping(out_stream.str()));

    std::ifstream file("../tests/assets/map.txt", std::ios::in);
    BinaryMapping bm2(file);
    EXPECT_EQ(bm2.to_table(), "00\t011\n"
                              "01\t110\n"
                              "10\t000\n"
                              "11\t100\n");

    std::ifstream file1("../tests/assets/sub.txt", std::ios::in);
    EXPECT_THROW((BinaryMapping(file1)), BMException);

    std::ifstream file2("../tests/assets/qc.txt", std::ios::in);
    EXPECT_THROW((BinaryMapping(file2)), BMException);
}

TEST(BinaryMapping, Substitutions) {
    Substitution sub("3 0 2 1");
    BinaryMapping bm(sub);
    EXPECT_EQ(bm, BinaryMapping(cf_set({BooleanFunction("1010"), BooleanFunction("1001")})));
    EXPECT_TRUE(sub == bm);
    EXPECT_TRUE(bm == sub);

    BinaryMapping bm1 = sub;
    EXPECT_EQ(bm1, BinaryMapping(cf_set({BooleanFunction("1010"), BooleanFunction("1001")})));
}
