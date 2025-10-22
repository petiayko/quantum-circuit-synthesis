#include <algorithm>
#include <iostream>
#include <map>
#include <string>

#include "computings.hpp"


using configuration = std::map<std::string, std::string>;

static const std::string version = "1.1.0";
static const std::string program_name = "QCS";

void print_program_info() {
    std::cout << program_name << " v" << version << std::endl;
    std::cout << "(c) Peter Makretskiy, IU8 BMSTU, 2025." << std::endl;
    std::cout << "Program for synthesizing quantum circuits from mapping"
              << std::endl << std::endl;
}

void print_program_version() {
    std::cout << version << std::endl;
}

void print_program_help() {
    print_program_info();

    std::cout << "Generic options:" << std::endl;
    std::cout << "  -V, --version       print version" << std::endl;
    std::cout << "  -h, --help          produce help message" << std::endl;
    std::cout << "  -l, --log ARG       minimum level of logging: 'DEBUG', 'INFO', 'WARNING', 'ERROR', 'CRITICAL' "
                 "(default: 'ERROR')"
              << std::endl;
    std::cout << std::endl;

    std::cout << "Operating modes:" << std::endl;
    std::cout << "  -t, --type ARG      type of input ('tt' - truth table, 'sub' - substitution, "
                 "'qc' - quantum circuit)"
              << std::endl;
    std::cout << std::endl;

    std::cout << "Synthesis options:" << std::endl;
    std::cout << "  -a, --algo ARG      algorithm to synthesis quantum circuit ('dummy', 'rw')" << std::endl;
    std::cout << "  -r, --reduction     reduce the output circuit (default: false)" << std::endl;
    std::cout << std::endl;

    std::cout << "Parameters:" << std::endl;
    std::cout << "  -i, --input ARG     path to input file" << std::endl;
    std::cout << "  -o, --output ARG    path to output file (default: prints into standard output)" << std::endl;
    std::cout << std::endl;
}

configuration parse_arguments(int argc, char *argv[]) {
    const std::map<std::string, std::string> arguments_map = {
            {"-V",          "--version"},
            {"--version",   "--version"},
            {"-h",          "--help"},
            {"--help",      "--help"},
            {"-l",          "--log"},
            {"--log",       "--log"},
            {"-t",          "--type"},
            {"--type",      "--type"},
            {"-a",          "--algo"},
            {"--algo",      "--algo"},
            {"-r",          "--reduction"},
            {"--reduction", "--reduction"},
            {"-i",          "--input"},
            {"--input",     "--input"},
            {"-o",          "--output"},
            {"--output",    "--output"},
    };
    std::map<std::string, bool> arguments_accounting = {
            {"--version",   false},
            {"--help",      false},
            {"--log",       false},
            {"--type",      false},
            {"--algo",      false},
            {"--reduction", false},
            {"--input",     false},
            {"--output",    false},
    };

    configuration config;

    for (int i = 1; i < argc;) {
        std::string arg = argv[i];

        auto it = arguments_map.find(arg);
        if (it == arguments_map.end()) {
            throw ArgumentException("Unknown argument: " + arg);
        }

        std::string full_arg_name = it->second;
        if (arguments_accounting[full_arg_name]) {
            throw ArgumentException("Duplicate argument: " + arg);
        }
        arguments_accounting[full_arg_name] = true;
        if (i + 1 >= argc || argv[i + 1][0] == '-') {
            if (full_arg_name == "--version" || full_arg_name == "--help" || full_arg_name == "--reduction") {
                config[full_arg_name] = "";
                i++;
            } else {
                throw ArgumentException("Missing value for argument " + arg);
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
    } catch (const ArgumentException &e) {
        LOG_ERROR("Processing parameters", std::string("Error: ") + e.what());
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
        LOG_ERROR("Processing parameters", "Path to input file was not provided");
        return 1;
    }
    auto input = it->second;
    trim(input);

    it = config.find("--type");
    if (it == config.end()) {
        LOG_ERROR("Processing parameters", "Type of input was not provided");
        return 1;
    }
    auto type_s = it->second;
    InputType type = InputType::EMPTY;
    trim(type_s);
    to_lower(type_s);

    if (type_s == "tt") {
        type = InputType::TABLE;
    } else if (type_s == "sub") {
        type = InputType::SUBSTITUTION;
    } else if (type_s == "qc") {
        type = InputType::CIRCUIT;
    } else if (!type_s.empty()) {
        type = InputType::UNKNOWN;
    }

    it = config.find("--algo");
    std::string algo_s;
    Algo algo = Algo::EMPTY;
    if (it != config.end()) {
        algo_s = it->second;
        trim(algo_s);
        to_lower(algo_s);
    }

    if (algo_s == "dummy") {
        algo = Algo::DUMMY;
    } else if (algo_s == "rw") {
        algo = Algo::RW;
    } else if (!algo_s.empty()) {
        algo = Algo::UNKNOWN;
    }

    bool reduction = config.count("--reduction");

    it = config.find("--output");
    std::string output;
    if (it != config.end()) {
        output = it->second;
        trim(output);
    }

    it = config.find("--log");
    std::string log_level_s = "error";
    if (it != config.end()) {
        log_level_s = it->second;
        trim(log_level_s);
        to_lower(log_level_s);
    }
    LogLevel log_level;
    if (log_level_s == "critical") {
        log_level = LogLevel::CRITICAL;
    } else if (log_level_s == "error") {
        log_level = LogLevel::ERROR;
    } else if (log_level_s == "warning") {
        log_level = LogLevel::WARNING;
    } else if (log_level_s == "info") {
        log_level = LogLevel::INFO;
    } else if (log_level_s == "debug") {
        log_level = LogLevel::DEBUG;
    } else {
        LOG_ERROR("Processing parameters", "Wrong log level was provided");
        return -1;
    }
    Logger::get_instance().set_level(log_level);

    LOG_INFO("Starting", "");
    try {
        process_config(type, algo, reduction, input, output);
    } catch (const std::exception &e) {
        LOG_ERROR("Finishing", std::string("Error: ") + e.what());
    }
    LOG_INFO("Finishing", "");
    return 0;
}
