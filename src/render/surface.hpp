#pragma once

#include "global.hpp"

namespace game
{
    void createSurface()
    {
        VK_CHECK_RESULT(glfwCreateWindowSurface(instance, window->window, nullptr, &surface));
    }
} // namespace game
