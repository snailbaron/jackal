#include "client/app.hpp"
#include <exception>
#include <iostream>

int main(int argc, char *argv[])
{
    try {
        App::initialize("jackal.json");
        app().run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
