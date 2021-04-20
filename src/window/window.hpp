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
    void update();
    ~Window();
    bool shouldClose();
    
    bool keyDown(int);
    bool buttonDown(int);

    glm::vec2 mousePos;
    glm::vec2 mouseDelta;
    
};
}  // namespace game