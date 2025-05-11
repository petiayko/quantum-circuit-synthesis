#include <algorithm>
#include <iostream>
#include <map>
#include <string>

using configuration = std::map<std::string, std::string>;

static const std::string version = "1.0";
static const std::string program_name = "qcs";

void print_program_info() {
    std::cout << program_name << " v" << version << std::endl;
    std::cout << "(c) Peter Makretskiy, IU8 BMSTU, 2025." << std::endl;
    std::cout << "program for synthesizing quantum circuits from input "
                 "mapping" << std::endl << std::endl;
}

void print_program_version() {
    std::cout << version << std::endl;
}

void print_program_help() {
    print_program_info();

    std::cout << "Generic options:" << std::endl;
    std::cout << "  --version       print version std::string" << std::endl;
    std::cout << "  --help          produce help message" << std::endl;
    std::cout << "  --log arg       minimum level of logging ('DEBUG', 'INFO', 'ERROR', 'CRITICAL')" << std::endl;
    std::cout << std::endl;

    std::cout << "Operating modes:" << std::endl;
    std::cout << "  --type arg      type of input ('tt' truth table, 'sub' substitution, 'qs' quantum scheme)"
              << std::endl;
    std::cout << std::endl;

    std::cout << "Synthesis options:" << std::endl;
    std::cout << "  --algo arg      algorithm to synthesis quantum scheme ('1', '2', '3')" << std::endl;
    std::cout << std::endl;

    std::cout << "Parameters:" << std::endl;
    std::cout << "  --input arg     path to input file" << std::endl;
    std::cout << "  --output arg    path to output file" << std::endl;
    std::cout << std::endl;
}

configuration parse_arguments(int argc, char *argv[]) {
    const std::map<std::string, std::string> arguments_map = {
            {"--version", "--version"},
            {"--help",    "--help"},
            {"-l",        "--log"},
            {"--log",     "--log"},
            {"-t",        "--type"},
            {"--type",    "--type"},
            {"-a",        "--algo"},
            {"--algo",    "--algo"},
            {"-i",        "--input"},
            {"--input",   "--input"},
            {"-o",        "--output"},
            {"--output",  "--output"},
    };
    std::map<std::string, bool> arguments_accounting = {
            {"--version", false},
            {"--help",    false},
            {"--log",     false},
            {"--type",    false},
            {"--algo",    false},
            {"--input",   false},
            {"--output",  false},
    };

    configuration config;

    for (int i = 1; i < argc;) {
        std::string arg = argv[i];

        auto it = arguments_map.find(arg);
        if (it == arguments_map.end()) {
            throw std::runtime_error("Unknown argument: " + arg);
        }

        std::string full_arg_name = it->second;
        if (arguments_accounting[full_arg_name]) {
            throw std::runtime_error("Duplicate argument: " + arg);
        }
        arguments_accounting[full_arg_name] = true;
        if (i + 1 >= argc || argv[i + 1][0] == '-') {
            if (full_arg_name == "--version" || full_arg_name == "--help") {
                config[full_arg_name] = "";
                i++;
            } else {
                throw std::runtime_error("Missing value for argument: " + arg);
            }
        } else {
            config[full_arg_name] = argv[i + 1];
            i += 2;
        }
    }

    return config;
}

int main(int argc, char *argv[]) {
    configuration config;
    try {
        config = parse_arguments(argc, argv);
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    if (config.empty()) {
        return 0;
    }

    auto it = config.find("--help");
    if (it != config.end()) {
        print_program_help();
        return 0;
    }

    it = config.find("--version");
    if (it != config.end()) {
        print_program_version();
        return 0;
    }

    it = config.find("--input");
    if (it == config.end()) {
        std::cerr << "Path to input file was not provided" << std::endl;
        return 1;
    }
    auto input = it->second;

    it = config.find("--type");
    if (it == config.end()) {
        std::cerr << "Type of input was not provided - impossible to define operating mode" << std::endl;
        return 1;
    }
    auto type = it->second;

    it = config.find("--algo");
    std::string algo;
    if (it != config.end()) {
        algo = it->second;
    }

    it = config.find("--output");
    std::string output;
    if (it != config.end()) {
        output = it->second;
    }

    it = config.find("--log");
    std::string log;
    if (it != config.end()) {
        log = it->second;
    }

    std::cout << input << " " << type << " " << output << " " << log << " " << algo << std::endl;
//    handle_config(type, algo, input, output, log);
//    let this function validate input
    return 0;
}
