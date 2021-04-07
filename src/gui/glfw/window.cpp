#include "window.hpp"

namespace game {

static void resizeCallback(GLFWwindow *window, int _width, int _height) {
    auto app = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
    app->resizedEvent = true;
    app->height = _height;
    app->width = _width;
}
Window::Window() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(width, height, "Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, resizeCallback);
}

bool Window::shouldClose(){
    return glfwWindowShouldClose(window);
}

void Window::performEvents(){
    glfwPollEvents();
}

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

}  // namespace game
