#include "gui/glfw/window.hpp"
#include "gui/vulkan/vulkan.hpp"

#include <iostream>
#include <cstdio>
#include <chrono>
#include <thread>

#define MaxFPS 30
#define MillisPerSec 1000
#define MillisPerFrame MillisPerSec / MaxFPS

game::Window* window;
game::Vulkan* vulkan;

void renderLoop() {
    window->performEvents();
    vulkan->drawFrame();
}

bool checkRunning() {
    return !window->shouldClose();
}

std::chrono::system_clock::time_point a = std::chrono::system_clock::now();
std::chrono::system_clock::time_point b = std::chrono::system_clock::now();

void logFPS(std::chrono::duration<double, std::milli> work_time) {
    int possibleFPS = int(MillisPerSec / work_time.count());
    int fps = std::min(possibleFPS, MaxFPS);
    
#ifdef DEBUG
    std::chrono::duration<double, std::milli> sleep_time = b - a;
    std::cout.precision(4);
    std::cout << "FPS: " << fps << " Possible: " << possibleFPS << " Work: " << std::fixed << work_time.count() << "ms Sleep: " << std::fixed << sleep_time.count() << "ms" << std::endl;
#else
    std::cout << "FPS: " << fps << " Possible: " << possibleFPS << std::endl;
#endif
}

void run() {
    while (checkRunning()) {
        a = std::chrono::system_clock::now();
        std::chrono::duration<double, std::milli> work_time = a - b;

        if (work_time.count() < MillisPerFrame) {
            std::chrono::duration<double, std::milli> delta_ms(MillisPerFrame - work_time.count());
            auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
            std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
        }
        b = std::chrono::system_clock::now();

        logFPS(work_time);
        renderLoop();
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
