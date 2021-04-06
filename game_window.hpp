#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>


namespace game {
    class GameWindow{
    public:
        GameWindow(int w, int h, std::string name);
        ~GameWindow();

        GameWindow(const GameWindow &) = delete;
        GameWindow &operator=(const GameWindow &) = delete;

        bool shouldClose() {return glfwWindowShouldClose(window); }

    private:
        void initWindow();
        const int width;
        const int height;

        std::string windowName;

        GLFWwindow *window;
    };
}



