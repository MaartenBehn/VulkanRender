#include "first_app.hpp"

namespace game {
    void FirstApp::run() {
        while (!gameWindow.shouldClose()) {
            glfwPollEvents();
        }
    }
}
