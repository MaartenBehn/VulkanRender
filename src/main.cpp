#include "window/window.hpp"
#include "render/render.hpp"

#include <cstdio>
#include <chrono>
#include <thread>

#define MaxFPS 30.0
#define MillisPerSec 1000.0
#define MillisPerFrame MillisPerSec / MaxFPS

game::Window* window;
game::Render* render;

float delta;
void renderLoop() {
    window->update();
    render->drawFrame(delta);
}

bool checkRunning() {
    return !window->shouldClose();
}

std::chrono::system_clock::time_point a = std::chrono::system_clock::now();
std::chrono::system_clock::time_point b = std::chrono::system_clock::now();

void logFPS(std::chrono::duration<double, std::milli> work_time) {
    double possibleFPS = MillisPerSec / work_time.count();
    double fps = std::min(possibleFPS, MaxFPS);
    
#ifdef DEBUG
    std::chrono::duration<double, std::milli> sleep_time = b - a;

    printf("FPS: %.0f Possible: %0.f Work: %.4fms Sleep: %.4fms\n", fps, possibleFPS, work_time.count(), sleep_time.count());
#else

    printf("FPS: %.0f Possible: %.0f\n", fps, possibleFPS);
#endif
}

void run() {
    while (checkRunning()) {
        a = std::chrono::system_clock::now();
        std::chrono::duration<double, std::milli> work_time = a - b;

        if (work_time.count() < MillisPerFrame) {
            delta = MillisPerFrame / MillisPerSec;

            std::chrono::duration<double, std::milli> delta_ms(MillisPerFrame - work_time.count());
            auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
            std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
        }
        else {
            delta = work_time.count() / MillisPerSec;
        }
        b = std::chrono::system_clock::now();

        logFPS(work_time);
        renderLoop();
    }
}

int main() {
    try {
        window = new game::Window();
        render = new game::Render(window);

        run();

        render->computeRun();

        render->~Render();
        window->~Window();
    } catch (const std::exception& e) {
        printf("%s\n", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}