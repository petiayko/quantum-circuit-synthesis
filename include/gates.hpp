#ifndef QUANTUM_CIRCUIT_SYNTHESIS_GATES_HPP
#define QUANTUM_CIRCUIT_SYNTHESIS_GATES_HPP

#include "primitives.hpp"
#include "strings.hpp"


#define GateType(type) static_cast<GateType>(type)

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

    void act(binary_vector &) const;

    void act(cf_set &) const;

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
    Circuit() = default;

    explicit Circuit(size_t, size_t = 0);

    explicit Circuit(const std::vector<Gate> &, size_t = 0);

    explicit Circuit(const std::string &);

    explicit Circuit(std::istream &);

    size_t dim() const noexcept;

    size_t memory() const noexcept;

    void set_memory(size_t);

    void act(binary_vector &) const;

    void act(cf_set &) const;

    void add(const Gate &);

    void insert(const Gate &, size_t = 0);

    bool simplify() noexcept;

    BinaryMapping produce_mapping() const noexcept;

    bool operator==(const Circuit &) const;

    friend std::ostream &operator<<(std::ostream &, const Circuit &) noexcept;

private:
    size_t dim_{};
    size_t memory_{};
    std::vector<Gate> gates_;

    void by_string_(const std::string &);
};

#endif //QUANTUM_CIRCUIT_SYNTHESIS_GATES_HPP
