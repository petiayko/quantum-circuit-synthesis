#ifndef QUANTUM_CIRCUIT_SYNTHESIS_EXSEPTIONS_HPP
#define QUANTUM_CIRCUIT_SYNTHESIS_EXSEPTIONS_HPP

#include <exception>
#include <string>

class ArgumentException : public std::exception {
public:
    ArgumentException() = default;

    explicit ArgumentException(const std::string &message) {
        message_ = message;
    }

    [[nodiscard]] const char *what() const noexcept override {
        return message_.c_str();
    }

private:
    std::string message_;
};

class IOException : public std::exception {
public:
    IOException() = default;

    explicit IOException(const std::string &message) {
        message_ = message;
    }

    [[nodiscard]] const char *what() const noexcept override {
        return message_.c_str();
    }

private:
    std::string message_;
};

class BFException : public std::exception {
public:
    BFException() = default;

    explicit BFException(const std::string &message) {
        message_ = message;
    }

    [[nodiscard]] const char *what() const noexcept override {
        return message_.c_str();
    }

private:
    std::string message_;
};

class BMException : public std::exception {
public:
    BMException() = default;

    explicit BMException(const std::string &message) {
        message_ = message;
    }

    [[nodiscard]] const char *what() const noexcept override {
        return message_.c_str();
    }

private:
    std::string message_;
};

class SubException : public std::exception {
public:
    SubException() = default;

    explicit SubException(const std::string &message) {
        message_ = message;
    }

    [[nodiscard]] const char *what() const noexcept override {
        return message_.c_str();
    }

private:
    std::string message_;
};

class GateException : public std::exception {
public:
    GateException() = default;

    explicit GateException(const std::string &message) {
        message_ = message;
    }

    [[nodiscard]] const char *what() const noexcept override {
        return message_.c_str();
    }

private:
    std::string message_;
};

class CircuitException : public std::exception {
public:
    CircuitException() = default;

    explicit CircuitException(const std::string &message) {
        message_ = message;
    }

    [[nodiscard]] const char *what() const noexcept override {
        return message_.c_str();
    }

private:
    std::string message_;
};

class SynthException : public std::exception {
public:
    SynthException() = default;

    explicit SynthException(const std::string &message) {
        message_ = message;
    }

    [[nodiscard]] const char *what() const noexcept override {
        return message_.c_str();
    }

private:
    std::string message_;
};

class MathException : public std::exception {
public:
    MathException() = default;

    explicit MathException(const std::string &message) {
        message_ = message;
    }

    [[nodiscard]] const char *what() const noexcept override {
        return message_.c_str();
    }

private:
    std::string message_;
};

class StringException : public std::exception {
public:
    StringException() = default;

    explicit StringException(const std::string &message) {
        message_ = message;
    }

    [[nodiscard]] const char *what() const noexcept override {
        return message_.c_str();
    }

private:
    std::string message_;
};

#endif //QUANTUM_CIRCUIT_SYNTHESIS_EXSEPTIONS_HPP
