#include "app.hpp"
#include <exception>
#include <iostream>

int main(int, char*[])
{
    App::initialize("jackal.json");
    app().run();

    return 0;
}
