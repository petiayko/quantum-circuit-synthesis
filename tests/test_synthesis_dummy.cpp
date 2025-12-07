#include <gtest/gtest.h>

#include "synthesis.hpp"


TEST(Synthesis, MappingDummy) {
    JobsConfig::instance().set(std::thread::hardware_concurrency());

    {
        BinaryMapping bm(table{{0, 0}});
        Circuit c("Lines: 2; 1");
        EXPECT_EQ(dummy_algorithm(bm), c);
        EXPECT_EQ(synthesize(bm, Algo::DUMMY), c);
        EXPECT_EQ(synthesize(bm, Algo::DUMMY, true), c);
    }
    {
        BinaryMapping bm(table{{1, 1}});
        Circuit c("Lines: 2; 1\nNOT(1)");
        EXPECT_EQ(dummy_algorithm(bm), c);
        EXPECT_EQ(synthesize(bm, Algo::DUMMY), c);
        EXPECT_EQ(synthesize(bm, Algo::DUMMY, true), c);
    }
    {
        BinaryMapping bm(table{{0, 0, 0, 0, 0, 0, 0, 0}});
        Circuit c("Lines: 4; 1");
        EXPECT_EQ(dummy_algorithm(bm), c);
        EXPECT_EQ(synthesize(bm, Algo::DUMMY), c);
        EXPECT_EQ(synthesize(bm, Algo::DUMMY, true), c);
    }
    {
        BinaryMapping bm(table{{1, 1, 1, 1, 1, 1, 1, 1}});
        Circuit c("Lines: 4; 1\nNOT(3)");
        EXPECT_EQ(dummy_algorithm(bm), c);
        EXPECT_EQ(synthesize(bm, Algo::DUMMY), c);
        EXPECT_EQ(synthesize(bm, Algo::DUMMY, true), c);
    }
    {
        BinaryMapping bm(table{{1, 0, 0, 0}});
        Circuit c("Lines: 3; 1\nNOT(2)\nkCNOT(2; 1)\nkCNOT(2; 0)\nkCNOT(2; 0, 1)");
        EXPECT_EQ(dummy_algorithm(bm), c);
        EXPECT_EQ(synthesize(bm, Algo::DUMMY), c);
        EXPECT_EQ(synthesize(bm, Algo::DUMMY, true), c);
    }
    {
        BinaryMapping bm(table{{1, 0, 0, 0},
                               {0, 0, 1, 0}});
        Circuit c("Lines: 4; 2\n"
                  "NOT(2)\nkCNOT(2; 1)\nkCNOT(2; 0)\nkCNOT(2; 0, 1)\n"
                  "kCNOT(3; 0)\nkCNOT(3; 0, 1)");
        EXPECT_EQ(dummy_algorithm(bm), c);
        EXPECT_EQ(synthesize(bm, Algo::DUMMY), c);
        EXPECT_EQ(synthesize(bm, Algo::DUMMY, true), c);
    }
    {
        BinaryMapping bm(table{{1, 0, 0, 0},
                               {0, 0, 1, 0},
                               {0, 0, 1, 0},
                               {1, 1, 1, 0},
                               {1, 0, 1, 0}});
        Circuit c("Lines: 7; 5\n"
                  "NOT(2)\nkCNOT(2; 1)\nkCNOT(2; 0)\nkCNOT(2; 0, 1)\n"
                  "kCNOT(3; 0)\nkCNOT(3; 0, 1)\n"
                  "kCNOT(4; 0)\nkCNOT(4; 0, 1)\n"
                  "NOT(5)\nkCNOT(5; 0, 1)\n"
                  "NOT(6)\nkCNOT(6; 1)");
        EXPECT_EQ(dummy_algorithm(bm), c);
        EXPECT_EQ(synthesize(bm, Algo::DUMMY), c);
        EXPECT_EQ(synthesize(bm, Algo::DUMMY, true), c);
    }
}

TEST(Synthesis, SubstitutionDummy) {
    JobsConfig::instance().set(std::thread::hardware_concurrency());

    EXPECT_THROW(dummy_algorithm(Substitution("0 2 1")), SynthException);
    {
        Substitution sub("0 1");
        Circuit c_etalon("Lines: 2; 1\nkCNOT(1; 0)");
        Circuit c = dummy_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::DUMMY), c);
        EXPECT_EQ(synthesize(sub, Algo::DUMMY, true), c);
        EXPECT_EQ(c_etalon, c);
    }
    {
        Substitution sub("1 0");
        Circuit c_etalon("Lines: 2; 1\nNOT(1)\nkCNOT(1; 0)");
        Circuit c = dummy_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::DUMMY), c);
        EXPECT_EQ(synthesize(sub, Algo::DUMMY, true), c);
        EXPECT_EQ(c_etalon, c);
    }
    {
        Substitution sub("3 2 0 1");
        Circuit c_etalon("Lines: 4; 2\n"
                         "NOT(2)\nkCNOT(2; 0)\n"
                         "NOT(3)\nkCNOT(3; 1)\nkCNOT(3; 0)");
        Circuit c = dummy_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::DUMMY), c);
        EXPECT_EQ(synthesize(sub, Algo::DUMMY, true), c);
        EXPECT_EQ(c_etalon, c);
    }
    {
        Substitution sub("5 7 2 1 6 3 0 4");
        Circuit c_etalon("Lines: 6; 3\n"
                         "NOT(3)\nkCNOT(3; 1)\nkCNOT(3; 0, 2)\n"
                         "kCNOT(4; 2)\nkCNOT(4; 1)\nkCNOT(4; 0)\nkCNOT(4; 0, 2)\n"
                         "NOT(5)\nkCNOT(5; 1)\nkCNOT(5; 1, 2)\nkCNOT(5; 0)\nkCNOT(5; 0, 2)\nkCNOT(5; 0, 1)\n");
        Circuit c = dummy_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::DUMMY), c);
        EXPECT_EQ(synthesize(sub, Algo::DUMMY, true), c);
        EXPECT_EQ(c_etalon, c);
    }
    {
        Substitution sub("2 5 4 1 0 6 7 3");
        Circuit c = dummy_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::DUMMY), c);
        EXPECT_EQ(synthesize(sub, Algo::DUMMY, true), c);
    }
    {
        Substitution sub("3 6 9 4 11 0 15 13 1 14 12 8 2 5 10 7");
        Circuit c = dummy_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::DUMMY), c);
        EXPECT_EQ(synthesize(sub, Algo::DUMMY, true), c);
    }
    {
        Substitution sub("29 23 25 10 31 30 9 12 2 19 0 20 24 26 14 1 11 22 21 5 27 6 17 4 16 8 18 28 15 13 7 3");
        Circuit c = dummy_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::DUMMY), c);
        EXPECT_EQ(synthesize(sub, Algo::DUMMY, true), c);
    }
    {
        Substitution sub(
                "153 167 111 228 182 72 225 33 248 166 192 142 26 23 21 57 157 239 178 152 179 69 250 120 25 24 202 "
                "122 253 217 141 8 144 172 12 213 30 236 98 137 87 249 188 243 168 18 255 70 220 47 191 242 199 54 "
                "147 149 161 40 234 133 41 187 200 193 194 195 79 59 104 244 42 138 22 10 201 81 154 58 102 74 125 "
                "158 93 110 170 6 39 128 136 100 11 107 127 216 105 204 91 183 210 247 246 5 65 151 224 89 223 229 "
                "67 176 0 186 203 160 88 114 82 181 34 254 190 134 28 164 211 62 163 132 226 121 196 1 48 19 184 175 "
                "123 177 117 233 37 108 222 139 129 173 86 103 140 84 146 49 162 130 212 235 219 71 45 53 119 143 174 "
                "209 156 3 17 46 237 38 169 61 135 251 113 31 252 63 75 2 109 9 238 44 197 101 29 99 112 83 232 77 35 "
                "16 66 198 94 97 106 85 241 148 20 4 205 43 68 52 206 124 32 185 171 95 78 36 13 15 126 73 115 150 "
                "116 55 218 60 245 207 96 27 208 231 227 92 64 50 240 56 189 7 51 145 76 90 180 221 215 131 214 155 "
                "118 159 230 165 80 14");
        Circuit c = dummy_algorithm(sub);
        EXPECT_EQ(synthesize(sub, Algo::DUMMY), c);
        EXPECT_EQ(synthesize(sub, Algo::DUMMY, true), c);
    }
}
