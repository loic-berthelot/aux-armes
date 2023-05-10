#pragma once
#include <stdexcept>

class Exception : public std::exception {
    std::string _message;
public:
    Exception(std::string const & m) : std::exception(), _message(m) {}
    const char * what() const noexcept override { return _message.c_str(); }
};