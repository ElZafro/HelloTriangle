#include "HelloTriangleApp.h"

//std
#include <iostream>
#include <stdexcept>
#include <cstdlib>

int main(int argc, char const *argv[])
{
    HT::HelloTriangleApp app;

    try
    {
        app.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
