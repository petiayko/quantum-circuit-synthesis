#include <gtest/gtest.h>

#include "primitives.hpp"

TEST(Mappings, Constructor) {
    EXPECT_THROW(Mapping(std::vector<std::vector<bool>>({})), std::runtime_error);
    EXPECT_THROW(Mapping(std::vector<std::vector<bool>>({{}})), std::runtime_error);
    EXPECT_THROW(Mapping(std::vector<std::vector<bool>>({{1},
                                                         {0},
                                                         {}})), std::runtime_error);
    EXPECT_THROW(Mapping(std::vector<std::vector<bool>>({{1},
                                                         {0, 1},
                                                         {1}})), std::runtime_error);
    EXPECT_THROW(Mapping(std::vector<std::vector<bool>>({{1, 1},
                                                         {0, 1},
                                                         {1}})), std::runtime_error);
    EXPECT_THROW(Mapping(std::vector<std::vector<bool>>({{1, 1, 1},
                                                         {1, 0, 1},
                                                         {1, 0, 0}})), std::runtime_error);

    EXPECT_THROW(Mapping(std::vector<BooleanFunction>({})), std::runtime_error);
    EXPECT_THROW(Mapping(std::vector<BooleanFunction>({BooleanFunction("1001"), BooleanFunction("10")})),
                 std::runtime_error);
    EXPECT_THROW(
            Mapping(std::vector<BooleanFunction>({BooleanFunction("01"), BooleanFunction("10"), BooleanFunction("0")})),
            std::runtime_error);

//    EXPECT_THROW(Mapping(""), std::runtime_error);
//    EXPECT_THROW(Mapping("001\n110\n1 0"), std::runtime_error);
//    EXPECT_THROW(Mapping("001\n110\n1"), std::runtime_error);
//    EXPECT_THROW(Mapping("0 0 1\n1 1 0\n1 0 1"), std::runtime_error);
}
