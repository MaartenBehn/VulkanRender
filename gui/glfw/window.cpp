#include "window.hpp"

namespace game {
Window::Window() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(width, height, "Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, resizeCallback);
}

Window::~Window() {
}

void Window::resizeCallback(GLFWwindow *window, int _width, int _height) {
    auto app = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
    app->resizedEvent = true;
    app->height = _height;
    app->width = _width;
}

}  // namespace game