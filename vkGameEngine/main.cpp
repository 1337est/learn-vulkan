#include "app.hpp"

#include <cstdlib>
#include <exception>
#include <iostream>

int main()
{
    vge::App app{};

    try
    {
        app.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
