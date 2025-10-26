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

// true - direct; false - inverted
using control = std::pair<size_t, bool>;

class Gate {
public:
    Gate() = default;

    explicit Gate(GateType, const std::vector<size_t> &, const std::vector<control> &, size_t);

    explicit Gate(const std::string &, size_t);

    size_t dim() const noexcept;

    GateType type() const noexcept;

    std::vector<size_t> nests() const noexcept;

    std::vector<size_t> controls() const noexcept;

    bool is_commutes(const Gate&) const noexcept;

    void act(binary_vector &) const;

    void act(cf_set &) const;

    bool operator==(const Gate &) const;

    friend std::ostream &operator<<(std::ostream &, const Gate &) noexcept;

private:
    GateType type_{};
    size_t dim_{};
    std::vector<size_t> nests_;
    std::vector<control> controls_;

    void init_(GateType, const std::vector<size_t> &, const std::vector<control> &, size_t);
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

    Circuit reduce() const noexcept;

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
