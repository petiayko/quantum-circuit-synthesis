#ifndef QUANTUM_CIRCUIT_SYNTHESIS_COMPUTINGS_HPP
#define QUANTUM_CIRCUIT_SYNTHESIS_COMPUTINGS_HPP

#include <filesystem>

#include "exseptions.hpp"
#include "logger.hpp"
#include "synthesis.hpp"


bool overwrite_confirmation() {
    std::cout << "Output file is already exists. Do you want to overwrite it [y/n]? ";
    std::string answer;
    std::cin >> answer;
    return !(answer == "n" || answer == "N");
}

template<typename T>
void write_result(const std::string &output_path, const T &result) {
    if (output_path.empty()) {
        std::cout << result << std::endl;
        return;
    }

    if (std::filesystem::exists(output_path)) {
        LOG_WARNING("Writing result", "Output file already exists");
        if (!overwrite_confirmation()) {
            LOG_WARNING("Writing result", "Will be written to cout");
            std::cout << result << std::endl;
            return;
        }
        LOG_WARNING("Writing result", "File will be overwritten");
    }

    std::fstream file(output_path, std::ios::out);
    if (!file) {
        throw IOException("Impossible to use this file");
    }
    file << result;
    file.close();
}

void process_config(const std::string &type, const std::string &algo, const std::string &input_path,
                    const std::string &output_path) {
    if (input_path.empty()) {
        throw ArgumentException("Path to input file was not provided");
    }
    std::ifstream file(input_path, std::ios::in);
    if (!file.is_open()) {
        throw IOException("Unable to open input file: " + input_path);
    }
    std::stringstream file_content;
    file_content << file.rdbuf();
    file.close();

    if (type == "qc") {
        if (!algo.empty()) {
            throw ArgumentException("Algo was provided for reverse mode");
        }
        LOG_INFO("Starting reverse of quantum circuit", "");
        Circuit c(file_content);

        if (c.memory()) {
            LOG_INFO("Starting reverse of quantum circuit", "The quantum circuit has additional memory");
            LOG_WARNING("Starting reverse of quantum circuit", "The result will be written as a binary mapping");
        }

        cf_set vec_bf;
        vec_bf.reserve(c.dim());
        for (size_t i = 0; i < c.dim(); i++) {
            vec_bf.emplace_back(i, c.dim());
        }
        c.act(vec_bf);

        LOG_INFO("Finishing reverse of quantum circuit", "");
        BinaryMapping bm(vec_bf);
        if (!c.memory()) {
            write_result<Substitution>(output_path, bm);
            return;
        }
        write_result<BinaryMapping>(output_path, bm);
        return;
    }

    if (algo.empty()) {
        throw ArgumentException("Synthesis algorithm was not provided");
    }
    if (algo == "dummy") {
        LOG_WARNING("Application parameters", "Selected synthesis algorithm is 'dummy', it always builds a quantum "
                                              "circuit with additional memory");
        LOG_WARNING("Application parameters", "Selected synthesis algorithm is 'dummy', number of additional memory "
                                              "lines in the resulting quantum circuit will be equal to the number of "
                                              "coordinate functions");
    } else if (algo != "rw") {
        throw ArgumentException("Unknown synthesis algorithm: " + algo);
    }

    LOG_INFO("Starting quantum circuit synthesis", "");
    if (type == "tt") {
        BinaryMapping bm(file_content);
        Circuit c = synthesize(bm, algo);
        if (c.memory() && algo == "rw") {
            LOG_INFO("Performing quantum circuit synthesis", "Provided binary mapping is not reversible");
            LOG_WARNING("Performing quantum circuit synthesis",
                        "Resulting quantum circuit will have additional memory");
        }
        write_result<Circuit>(output_path, c);
    } else if (type == "sub") {
        Substitution sub(file_content);
        Circuit c = synthesize(sub, algo);
        write_result<Circuit>(output_path, c);
    } else {
        throw ArgumentException("Unknown type of input: " + type);
    }
    LOG_INFO("Finishing quantum circuit synthesis", "");
}

#endif //QUANTUM_CIRCUIT_SYNTHESIS_COMPUTINGS_HPP
