#ifndef QUANTUM_CIRCUIT_SYNTHESIS_COMPUTINGS_HPP
#define QUANTUM_CIRCUIT_SYNTHESIS_COMPUTINGS_HPP

#include "gates.hpp"

void process_config(const std::string &type, const std::string &algo, const std::string &input_path,
                    const std::string &output_path, const std::string &log_level) {
    std::cout << algo << ' ' << output_path << ' ' << log_level << std::endl;
    if (input_path.empty()) {
        throw std::runtime_error{"Path to input file was not provided"};
    }
    std::ifstream file(input_path, std::ios::in);
    if (!file.good()) {
        throw std::runtime_error{std::string("File not exist: " + input_path)};
    }

    if (type == "tt") {
        BinaryMapping bm(file);
        file.close();
    } else if (type == "sub") {
        Substitution sub(file);
        file.close();
    } else if (type == "qc") {
        Circuit c(file);
        file.close();

        std::vector<BooleanFunction> vec_bf;
        for (size_t i = 0; i < c.dim(); i++) {
            vec_bf.push_back(BooleanFunction(i, c.dim()));
        }
        c.act(vec_bf);

        Substitution sub(vec_bf);
        std::cout << sub << std::endl;
    } else {
        throw std::runtime_error{std::string("Unknown type of input: ") + type};
    }
}

#endif //QUANTUM_CIRCUIT_SYNTHESIS_COMPUTINGS_HPP
