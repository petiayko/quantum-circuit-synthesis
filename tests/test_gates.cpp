#include <gtest/gtest.h>

#include "gates.hpp"

TEST(Gates, Constructor) {
    EXPECT_THROW(Gate(-1, {}, {}, 2), std::runtime_error);
    EXPECT_THROW(Gate(5, {}, {}, 2), std::runtime_error);
    EXPECT_THROW(Gate(NOT, {4}, {}, 1), std::runtime_error);

    // NOT
    EXPECT_THROW(Gate(NOT, {}, {}, 5), std::runtime_error);
    EXPECT_THROW(Gate(NOT, {}, {1}, 5), std::runtime_error);
    EXPECT_THROW(Gate(NOT, {0}, {1}, 5), std::runtime_error);
    EXPECT_THROW(Gate(NOT, {0, 2}, {}, 5), std::runtime_error);

    //CNOT
    EXPECT_THROW(Gate(CNOT, {}, {1}, 5), std::runtime_error);
    EXPECT_THROW(Gate(CNOT, {1}, {}, 5), std::runtime_error);
    EXPECT_THROW(Gate(CNOT, {1}, {1}, 5), std::runtime_error);
    EXPECT_THROW(Gate(CNOT, {0, 1}, {1}, 5), std::runtime_error);
    EXPECT_THROW(Gate(CNOT, {0}, {1, 2}, 5), std::runtime_error);

    //kCNOT
    EXPECT_THROW(Gate(kCNOT, {}, {1}, 5), std::runtime_error);
    EXPECT_THROW(Gate(kCNOT, {1}, {}, 5), std::runtime_error);
    EXPECT_THROW(Gate(kCNOT, {1}, {1}, 5), std::runtime_error);
    EXPECT_THROW(Gate(kCNOT, {1}, {1, 4}, 5), std::runtime_error);
    EXPECT_THROW(Gate(kCNOT, {0, 1}, {1}, 5), std::runtime_error);
    EXPECT_NO_THROW(Gate(kCNOT, {0}, {1, 3, 2, 4}, 5));

    // SWAP
    EXPECT_THROW(Gate(SWAP, {}, {1}, 5), std::runtime_error);
    EXPECT_THROW(Gate(SWAP, {1}, {}, 5), std::runtime_error);
    EXPECT_THROW(Gate(SWAP, {1}, {}, 5), std::runtime_error);
    EXPECT_THROW(Gate(SWAP, {0, 1}, {1}, 5), std::runtime_error);
    EXPECT_THROW(Gate(SWAP, {1, 1}, {}, 5), std::runtime_error);

    // CSWAP
    EXPECT_THROW(Gate(CSWAP, {}, {1}, 5), std::runtime_error);
    EXPECT_THROW(Gate(CSWAP, {1}, {}, 5), std::runtime_error);
    EXPECT_THROW(Gate(CSWAP, {1}, {}, 5), std::runtime_error);
    EXPECT_THROW(Gate(CSWAP, {0, 1}, {1}, 5), std::runtime_error);
    EXPECT_THROW(Gate(CSWAP, {1, 1}, {}, 5), std::runtime_error);
    EXPECT_THROW(Gate(CSWAP, {1, 2}, {}, 5), std::runtime_error);
    EXPECT_THROW(Gate(CSWAP, {1, 2}, {2}, 5), std::runtime_error);
}
