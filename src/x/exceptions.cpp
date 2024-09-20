#include <x/exceptions.hpp>

#include <sstream>
#ifdef __cpp_lib_stacktrace
#include <stacktrace>
#endif

namespace x {

Error::Error(std::string_view message, std::source_location sl)
{
    auto stream = std::ostringstream{};
    stream <<
        sl.file_name() << ":" <<
        sl.line() << ":" << sl.column() <<
        " (" << sl.function_name() << "): " <<
        message;
#ifdef __cpp_lib_stacktrace
    stream << "\n" << std::stacktrace::current() << "\n";
#endif

    _message = std::move(stream).str();
}

const char* Error::what() const noexcept
{
    return _message.c_str();
}

} // namespace x