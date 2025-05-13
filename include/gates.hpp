#ifndef QUANTUM_CIRCUIT_SYNTHESIS_GATES_HPP
#define QUANTUM_CIRCUIT_SYNTHESIS_GATES_HPP

#include <string>

#include "primitives.hpp"
#include "strings.hpp"

static const int NOT = 0;
static const int CNOT = 1;
static const int kCNOT = 2;
static const int SWAP = 3;
static const int CSWAP = 4;

class Gate {
public:
    explicit Gate(int, const std::unordered_set<size_t> &, const std::unordered_set<size_t> &, size_t);

    explicit Gate(const std::string &, size_t);

    void act(std::vector<BooleanFunction> &) const noexcept;

    friend std::ostream &operator<<(std::ostream &, const Gate &) noexcept;

private:
    int type_;
    size_t dim_;
    std::unordered_set<size_t> nests_;
    std::unordered_set<size_t> controls_;

    void init_(int, const std::unordered_set<size_t> &, const std::unordered_set<size_t> &, size_t);
};


class Circuit {
public:
    explicit Circuit();

    explicit Circuit(const std::string &);

    explicit Circuit(std::istream &);

    void add(const Gate &);

    friend std::ostream &operator<<(std::ostream &, const Circuit &) noexcept;

private:
    std::vector<Gate> gates_;
};

#endif //QUANTUM_CIRCUIT_SYNTHESIS_GATES_HPP
