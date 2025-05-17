#ifndef QUANTUM_CIRCUIT_SYNTHESIS_LOGGER_HPP
#define QUANTUM_CIRCUIT_SYNTHESIS_LOGGER_HPP

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>
#include <mutex>

#define LOG_DEBUG(action, result)    Logger::get_instance().log(LogLevel::DEBUG, action, result)
#define LOG_INFO(action, result)     Logger::get_instance().log(LogLevel::INFO, action, result)
#define LOG_WARNING(action, result)  Logger::get_instance().log(LogLevel::WARNING, action, result)
#define LOG_ERROR(action, result)    Logger::get_instance().log(LogLevel::ERROR, action, result)
#define LOG_CRITICAL(action, result) Logger::get_instance().log(LogLevel::CRITICAL, action, result)

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL,
};

class Logger {
public:
    static Logger &get_instance() {
        static Logger instance;
        return instance;
    }

    void set_level(LogLevel level) {
        std::lock_guard<std::mutex> lock(mutex_);
        level_ = level;
    }

    void log(LogLevel level, const std::string &action, const std::string &result) {
        if (level >= level_) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (level >= LogLevel::ERROR) {
                std::cerr << current_time_to_string() << ' ' << to_string(level) << action << " - " << result
                          << std::endl;
            } else {
                std::cout << current_time_to_string() << ' ' << to_string(level) << action << " - " << result
                          << std::endl;
            }
        }
    }

private:
    LogLevel level_ = LogLevel::ERROR;
    std::mutex mutex_;

    static std::string to_string(LogLevel level) {
        switch (level) {
            case LogLevel::DEBUG:
                return "[DEBUG]    ";
            case LogLevel::INFO:
                return "[INFO]     ";
            case LogLevel::WARNING:
                return "[WARNING]  ";
            case LogLevel::ERROR:
                return "[ERROR]    ";
            case LogLevel::CRITICAL:
                return "[CRITICAL] ";
            default:
                return "[UNKNOWN]  ";
        }
    }

    static std::string current_time_to_string() {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        auto tm = *std::localtime(&time);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", &tm);
        return buffer;
    }
};

#endif //QUANTUM_CIRCUIT_SYNTHESIS_LOGGER_HPP
