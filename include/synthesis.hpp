#ifndef QUANTUM_CIRCUIT_SYNTHESIS_SYNTHESIS_HPP
#define QUANTUM_CIRCUIT_SYNTHESIS_SYNTHESIS_HPP

#include "exseptions.hpp"
#include "gates.hpp"
#include "logger.hpp"

enum class Algo {
    DUMMY,
    RW,
    SS,
    NSS,
    UNKNOWN = 1024,
    EMPTY = 2048,
};

std::vector<Gate> generate_all_gates(const std::vector<GateType> &, size_t);

std::vector<Gate> generate_all_gates(size_t);

std::vector<Gate> generate_all_gates(const std::vector<GateType> &, size_t, size_t, size_t);

std::vector<Gate> generate_all_gates(size_t, size_t);

Circuit synthesize(const BinaryMapping &, Algo = Algo::RW, bool = false);

Circuit synthesize(const Substitution &, Algo = Algo::RW, bool = false);

Circuit dummy_algorithm(const BinaryMapping &, bool = false);

Circuit dummy_algorithm(const Substitution &, bool = false);

Circuit RW_algorithm(const BinaryMapping &, bool = false);

Circuit RW_algorithm(const Substitution &, bool = false);

Circuit SS_algorithm(const BinaryMapping &, bool = false);

Circuit SS_algorithm(const Substitution &, bool = false);

Circuit NSS_algorithm(const BinaryMapping &, bool = false);

Circuit NSS_algorithm(const Substitution &, bool = false);

#endif //QUANTUM_CIRCUIT_SYNTHESIS_SYNTHESIS_HPP
