#pragma once

#include <exception>
#include <source_location>
#include <string>
#include <string_view>

class Error : public std::exception {
public:
    explicit Error(
        std::string_view message,
        std::source_location sl = std::source_location::current());

    const char* what() const noexcept override;

private:
    std::string _message;
};
