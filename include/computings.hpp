#ifndef QUANTUM_CIRCUIT_SYNTHESIS_COMPUTINGS_HPP
#define QUANTUM_CIRCUIT_SYNTHESIS_COMPUTINGS_HPP

#include "gates.hpp"
#include "logger.hpp"

template<typename T>
void write_result(const std::string &output_path, const T &result) {
    // @TODO if file exists ask user to overwrite it else create it
    if (output_path.empty()) {
        std::cout << result;
        std::cout << std::endl;
        return;
    }
    std::ofstream file(output_path, std::ios::out);
    if (!file.is_open()) {
        LOG_ERROR("Application parameters",
                  std::string("Unable to open output file: ") + output_path + std::string(" - write to cout"));
        std::cout << result << std::endl;
        throw std::runtime_error{std::string("Unable to open input file: ") + output_path};
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

    std::cout << algo << std::endl;
    if (type == "tt") {
        BinaryMapping bm(file_content);
    } else if (type == "sub") {
        Substitution sub(file_content);
    } else {
        LOG_ERROR("Application parameters", std::string("Unknown type of input: ") + type);
        throw std::runtime_error{std::string("Unknown type of input: ") + type};
    }
}

#endif //QUANTUM_CIRCUIT_SYNTHESIS_COMPUTINGS_HPP
