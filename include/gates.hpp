#ifndef QUANTUM_CIRCUIT_SYNTHESIS_GATES_HPP
#define QUANTUM_CIRCUIT_SYNTHESIS_GATES_HPP

#include <string>

static const std::string NOT    = "NOT";
static const std::string CNOT   = "CNOT";
static const std::string CCNOT  = "CCNOT";
static const std::string kCNOT  = "kCNOT";
static const std::string SWAP   = "SWAP";
static const std::string CSWAP  = "CSWAP";

class Gate {
};

class NotGate {
};

class ToffoliGate {
};

#endif //QUANTUM_CIRCUIT_SYNTHESIS_GATES_HPP
