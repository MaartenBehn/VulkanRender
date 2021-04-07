#pragma once

#include "../gui.hpp"

namespace game {
class Window {
   public:

    uint32_t width = 800;
    uint32_t height = 600;
    bool resizedEvent = false;
    GLFWwindow *window;

    Window();
    ~Window();
    bool shouldClose();
    void performEvents();
};
}  // namespace game