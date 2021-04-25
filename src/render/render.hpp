#pragma once

#include "../gui.hpp"
#include "../window/window.hpp"

namespace game
{
    class Render
    {
    public:
        Render(Window *_window);
        ~Render();
        void drawFrame(float delta);
        void waitForDeviceDone();
        void computeRun();
    };
} // namespace game