#ifndef QUANTUM_CIRCUIT_SYNTHESIS_STATISTICS_HPP
#define QUANTUM_CIRCUIT_SYNTHESIS_STATISTICS_HPP

#include "synthesis.hpp"

void generate_substitution(size_t n, Circuit (*synt_algo)(const Substitution &, bool),
                           std::vector<std::tuple<size_t, size_t, size_t, std::chrono::microseconds>> &statistic) {
    std::vector<size_t> sub(n);
    for (size_t i = 0; i < n; i++) {
        sub[i] = i;
    }
    size_t failed_count = 0;
    do {
        try {
            auto start = std::chrono::high_resolution_clock::now();
            auto c = synt_algo(Substitution(sub), true);
            auto end = std::chrono::high_resolution_clock::now();
            statistic.emplace_back(c.dim(), c.memory(), c.complexity(),
                                   duration_cast<std::chrono::microseconds>(end - start));
        } catch (std::exception &e) {
            std::cout << e.what() << std::endl;
            std::cout << Substitution(sub) << std::endl;
            failed_count++;
        }
    } while (std::next_permutation(sub.begin(), sub.end()));
    std::cout << "Failed: " << failed_count << std::endl;
}

std::tuple<double, double, double, double> collect_statistics(size_t power, Algo algo) {
    std::vector<std::tuple<size_t, size_t, size_t, std::chrono::microseconds>> statistic;
    switch (algo) {
        case (Algo::DUMMY):
            generate_substitution(power, dummy_algorithm, statistic);
            break;
        case Algo::RW:
            generate_substitution(power, RW_algorithm, statistic);
            break;
        case Algo::SS:
            generate_substitution(power, SS_algorithm, statistic);
            break;
        case Algo::ZKB:
            generate_substitution(power, ZKB_algorithm, statistic);
            break;
        case Algo::CA:
            generate_substitution(power, CA_algorithm, statistic);
            break;
        default:
            break;
    }

    size_t dim_sum = 0;
    size_t ancilla_sum = 0;
    size_t complexity_sum = 0;
    std::chrono::microseconds microseconds_sum{0};
    for (const auto &record: statistic) {
        dim_sum += std::get<0>(record);
        ancilla_sum += std::get<1>(record);
        complexity_sum += std::get<2>(record);
        microseconds_sum += std::get<3>(record);
    }

    return {static_cast<double>(dim_sum) / statistic.size(),
            static_cast<double>(ancilla_sum) / statistic.size(),
            static_cast<double>(complexity_sum) / statistic.size(),
            microseconds_sum.count() / statistic.size()};
}

void collect_statistics() {
    JobsConfig::instance().set(100);
    const std::vector<size_t> powers = {2, 4, 8};
    const std::vector<Algo> algorithms = {Algo::DUMMY, Algo::RW, Algo::SS, Algo::ZKB, Algo::CA};

    for (auto algo: algorithms) {
        for (auto power: powers) {
            auto results = collect_statistics(power, algo);
            std::cout << static_cast<int>(algo) << " algorithm (power is equal " << power << ")"
                      << std::endl
                      << "Mean dimension " << std::get<0>(results)
                      << std::endl
                      << "Mean additional memory lines count " << std::get<1>(results)
                      << std::endl
                      << "Mean complexity " << std::get<2>(results)
                      << std::endl
                      << "Mean duration, ms " << std::get<3>(results)
                      << std::endl
                      << std::endl;
        }
    }
}

#endif //QUANTUM_CIRCUIT_SYNTHESIS_STATISTICS_HPP
