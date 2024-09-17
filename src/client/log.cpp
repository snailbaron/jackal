#include "log.hpp"
#include <iostream>

std::ostream& log()
{
    return std::cerr;
}