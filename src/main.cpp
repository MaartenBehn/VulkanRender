#include "helloTringle.hpp"
#include "gui/glfw/window.hpp"
#include "gui/vulkan/vulkan.hpp"

game::Window* window;
game::Vulkan* vulkan;
void run() {
    while (!window->shouldClose()) {
        window->performEvents();
        vulkan->drawFrame();
    }
}

int main() {
    try {
        window = new game::Window();
        vulkan = new game::Vulkan(window);

        run();

        vulkan->waitForDeviceDone();

        vulkan->~Vulkan();
        window->~Window();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}