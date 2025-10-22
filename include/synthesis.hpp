#ifndef QUANTUM_CIRCUIT_SYNTHESIS_SYNTHESIS_HPP
#define QUANTUM_CIRCUIT_SYNTHESIS_SYNTHESIS_HPP

#include "exseptions.hpp"
#include "gates.hpp"

enum class Algo {
    DUMMY,
    RW,
    UNKNOWN = 1024,
    EMPTY = 2048,
};

Circuit
synthesize(const BinaryMapping &, Algo = Algo::RW, bool simplify = false);

Circuit synthesize(const Substitution &, Algo = Algo::RW, bool simplify = false);

Circuit dummy_algorithm(const BinaryMapping &);

Circuit dummy_algorithm(const Substitution &);

Circuit RW_algorithm(const BinaryMapping &);

Circuit RW_algorithm(const Substitution &);

#endif //QUANTUM_CIRCUIT_SYNTHESIS_SYNTHESIS_HPP
