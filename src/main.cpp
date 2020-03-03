
#include <iostream>

#include "vulkan-tutorial-app.hpp"

auto main() -> int
{
    VulkanTutorialApp app;

    try
    {
        app.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
