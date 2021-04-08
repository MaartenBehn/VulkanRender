namespace game
{
void createSurface() {
    if (glfwCreateWindowSurface(instance, window->window, nullptr, &surface) !=
        VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
    }
}
} // namespace game
