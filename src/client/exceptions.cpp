#include "sdl.hpp"
#include "exceptions.hpp"

#include <stacktrace>
#include <format>

Error::Error(std::string message, std::source_location sl)
    : _message(std::format(
        "{}:{}:{} ({}): {}\n{}\n",
        sl.file_name(),
        sl.line(),
        sl.column(),
        sl.function_name(),
        message,
        std::stacktrace::current()))
{ }

const char* Error::what() const noexcept
{
    return _message.c_str();
}