#ifndef QUANTUM_CIRCUIT_SYNTHESIS_COMPUTINGS_HPP
#define QUANTUM_CIRCUIT_SYNTHESIS_COMPUTINGS_HPP

#include <filesystem>

#include "gates.hpp"
#include "logger.hpp"

bool overwrite_confirmation() {
    std::cout << "Output file is already exists. Do you want to overwrite it [y/n]? ";
    std::string answer;
    std::cin >> answer;
    return (answer == "y" || answer == "Y");
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
        } else {
            LOG_WARNING("Writing result", "File will be overwritten");
        }
    }

    std::fstream file(output_path, std::ios::out);
    if (!file) {
        LOG_ERROR("Writing result", "Impossible to use this file");
        throw std::runtime_error{"Impossible to use this file"};
    }
    file << result;
    file.close();
}

void process_config(const std::string &type, const std::string &algo, const std::string &input_path,
                    const std::string &output_path) {
    if (input_path.empty()) {
        LOG_ERROR("Application parameters", "Path to input file was not provided");
        throw std::runtime_error{"Path to input file was not provided"};
    }
    std::ifstream file(input_path, std::ios::in);
    if (!file.is_open()) {
        LOG_ERROR("Application parameters", std::string("Unable to open input file: ") + input_path);
        throw std::runtime_error{std::string("Unable to open input file: ") + input_path};
    }
    std::stringstream file_content;
    file_content << file.rdbuf();
    file.close();

    if (type == "qc") {
        if (!algo.empty()) {
            LOG_ERROR("Application parameters", "Algo was provided for reverse mode");
            throw std::runtime_error{"Algo was provided for reverse mode"};
        }
        LOG_INFO("Starting converting of quantum circuit into substitution", "");
        Circuit c(file_content);

        std::vector<BooleanFunction> vec_bf;
        vec_bf.reserve(c.dim());
        for (size_t i = 0; i < c.dim(); i++) {
            vec_bf.emplace_back(i, c.dim());
        }
        c.act(vec_bf);

        Substitution sub(vec_bf);
        LOG_INFO("Finishing converting of quantum circuit into substitution", "");
        write_result<Substitution>(output_path, sub);
        return;
    }

    if (algo.empty()) {
        LOG_ERROR("Application parameters", "Synthesis algorithm was not provided");
        throw std::runtime_error{"Synthesis algorithm was not provided"};
    }
    if (algo == "enum") {
        LOG_WARNING("Application parameters", "Selected synthesis algorithm is enumeration (enum), it does not guarantee results and fast execution time");
    } else if (algo != "rw") {
        LOG_ERROR("Application parameters", std::string("Unknown synthesis algorithm: ") + algo);
        throw std::runtime_error{std::string("Unknown synthesis algorithm: ") + algo};
    }

    LOG_INFO("Starting of quantum circuit synthesis", "");
    if (type == "tt") {
        BinaryMapping bm(file_content);
        Circuit c = synthesize(bm, algo);
        write_result<Circuit>(output_path, c);
    } else if (type == "sub") {
        Substitution sub(file_content);
        Circuit c = synthesize(sub, algo);
        write_result<Circuit>(output_path, c);
    } else {
        LOG_ERROR("Application parameters", std::string("Unknown type of input: ") + type);
        throw std::runtime_error{std::string("Unknown type of input: ") + type};
    }
    LOG_INFO("Finishing of quantum circuit synthesis", "");
}

#endif //QUANTUM_CIRCUIT_SYNTHESIS_COMPUTINGS_HPP
