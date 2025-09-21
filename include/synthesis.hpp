#ifndef QUANTUM_CIRCUIT_SYNTHESIS_SYNTHESIS_HPP
#define QUANTUM_CIRCUIT_SYNTHESIS_SYNTHESIS_HPP

#include "exseptions.hpp"
#include "gates.hpp"

Circuit simplification(const Circuit &);

Circuit synthesize(const BinaryMapping &, const std::string & = "rw", bool simplify = false);

Circuit synthesize(const Substitution &, const std::string & = "rw", bool simplify = false);

Circuit dummy_algorithm(const BinaryMapping &);

Circuit dummy_algorithm(const Substitution &);

Circuit RW_algorithm(const BinaryMapping &);

Circuit RW_algorithm(const Substitution &);

#endif //QUANTUM_CIRCUIT_SYNTHESIS_SYNTHESIS_HPP
