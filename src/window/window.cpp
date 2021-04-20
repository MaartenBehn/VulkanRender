#include "window.hpp"
#include <cstdio>
#include <iostream>

namespace game
{

    static void resizeCallback(GLFWwindow *window, int _width, int _height)
    {
        auto app = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
        app->resizedEvent = true;
        app->height = _height;
        app->width = _width;
    }
    Window::Window()
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        window = glfwCreateWindow(width, height, "Vulkan", nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, resizeCallback);
    }

    void Window::update(){
        glfwPollEvents();

        // Mouse Pos
        glm::vec2 lastPos = mousePos;

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        mousePos = glm::vec2(xpos, ypos);

        mouseDelta = mousePos - lastPos;

        //std::cout << mousePos.x << " " << mousePos.y << " " << mouseDelta.x << " " << mouseDelta.y << std::endl;
    }

    bool Window::shouldClose()
    {
        return glfwWindowShouldClose(window);
    }

    bool Window::keyDown(int key)
    {
        return glfwGetKey(window, key) == GLFW_PRESS;
    }

    bool Window::buttonDown(int button)
    {
        return glfwGetMouseButton(window, button) == GLFW_PRESS;
    }

    Window::~Window()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

} // namespace game
