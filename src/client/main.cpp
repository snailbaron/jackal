#include "app.hpp"
#include "fs.hpp"

#include <cstdlib>
#include <exception>
#include <iostream>

int main(int, char*[]) try
{
    App::initialize(fs::exeDir() / "jackal.yaml");
    app().run();

    return 0;
} catch (const std::exception& e) {
    std::cerr << e.what() << "\n";
    return EXIT_FAILURE;
}