#pragma once

#include "../../util.hpp"
#include "../glfw/window.hpp"
#include "../gui.hpp"

namespace game {
class Vulkan {
   public:
    Vulkan(Window *_window);
    ~Vulkan();
    void drawFrame();
    void waitForDeviceDone();
};
}  // namespace game
