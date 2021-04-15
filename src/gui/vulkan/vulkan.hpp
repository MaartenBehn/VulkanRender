#pragma once

#include "../../util.hpp"
#include "../gui.hpp"
#include "../glfw/window.hpp"

namespace game
{
    class Vulkan
    {
    public:
        Vulkan(Window *_window);
        ~Vulkan();
        void drawFrame();
        void waitForDeviceDone();
    };
} // namespace game
