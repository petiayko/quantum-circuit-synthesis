#ifndef QUANTUM_CIRCUIT_SYNTHESIS_SYNTHESIS_HPP
#define QUANTUM_CIRCUIT_SYNTHESIS_SYNTHESIS_HPP

#include <atomic>
#include <future>

#include "exseptions.hpp"
#include "gates.hpp"
#include "logger.hpp"

class JobsConfig {
public:
    static JobsConfig &instance() {
        static JobsConfig config;
        return config;
    }

    size_t set(size_t jobs) {
        const auto max_jobs = std::thread::hardware_concurrency();
        if (jobs > max_jobs) {
            jobs = max_jobs;
        }
        std::atomic_ref(jobs_).store(jobs);
        return jobs;
    }

    [[nodiscard]] size_t get() const {
        return std::atomic_ref(jobs_).load();
    }

private:
    JobsConfig() = default;

    size_t jobs_ = 1;
};

enum class Algo {
    DUMMY,
    RW,
    SS,
    ZKB,
    CA,
    UNKNOWN = 1024,
    EMPTY = 2048,
};

size_t count_gates(GateType, size_t, bool = false) noexcept;

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

Circuit ZKB_algorithm(const BinaryMapping &, bool = false);

Circuit ZKB_algorithm(const Substitution &, bool = false);

Circuit CA_algorithm(const BinaryMapping &, bool = false);

Circuit CA_algorithm(const Substitution &, bool = false);

#endif //QUANTUM_CIRCUIT_SYNTHESIS_SYNTHESIS_HPP
