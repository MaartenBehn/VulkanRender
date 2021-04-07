#pragma once

#include <algorithm>  // Necessary for std::min/std::max
#include <cstdint>    // Necessary for UINT32_MAX
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <optional>
#include <set>
#include <stdexcept>
#include <vector>

#include "../../util.hpp"
#include "../gui.hpp"
#include "../glfw/window.hpp"
#include "vertex.hpp"

#ifdef DEBUG
const bool enableValidationLayers = true;
#else
const bool enableValidationLayers = false;
#endif

namespace game
{
    class Vulkan
    {
    public:
     Vulkan(Window* _window);
     ~Vulkan();
     void drawFrame();
     void waitForDeviceDone();
    };
} // namespace game