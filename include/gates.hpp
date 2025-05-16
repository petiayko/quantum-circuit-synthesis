#ifndef QUANTUM_CIRCUIT_SYNTHESIS_GATES_HPP
#define QUANTUM_CIRCUIT_SYNTHESIS_GATES_HPP

#include "primitives.hpp"
#include "strings.hpp"

enum class GateType {
    NOT = 0,
    CNOT = 1,
    kCNOT = 2,
    SWAP = 3,
    CSWAP = 4,
};

class Gate {
public:
    explicit Gate(GateType, const std::vector<size_t> &, const std::vector<size_t> &, size_t);

    explicit Gate(const std::string &, size_t);

    size_t dim() const noexcept;

    void act(std::vector<bool> &) const;

    void act(std::vector<BooleanFunction> &) const;

    bool operator==(const Gate &) const;

    friend std::ostream &operator<<(std::ostream &, const Gate &) noexcept;

private:
    GateType type_{};
    size_t dim_{};
    std::vector<size_t> nests_;
    std::vector<size_t> controls_;

    void init_(GateType, const std::vector<size_t> &, const std::vector<size_t> &, size_t);
};


class Circuit {
public:
    explicit Circuit(size_t);

    explicit Circuit(const std::vector<Gate> &);

    explicit Circuit(const Substitution &, int);

    explicit Circuit(const BinaryMapping &, int);

    explicit Circuit(const std::string &);

    explicit Circuit(std::istream &);

    size_t dim() const noexcept;

    void act(std::vector<bool> &) const;

    void act(std::vector<BooleanFunction> &) const;

    void add(const Gate &);

    bool operator==(const Circuit &) const;

    friend std::ostream &operator<<(std::ostream &, const Circuit &) noexcept;

private:
    size_t dim_{};
    std::vector<Gate> gates_;

    void by_string_(const std::string &);
};

#endif //QUANTUM_CIRCUIT_SYNTHESIS_GATES_HPP
