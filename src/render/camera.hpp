#pragma once

#include "global.hpp"

/*
* Basic camera class
*
* Copyright (C) 2016 by Sascha Willems - www.saschawillems.de
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace game
{


    void updateCamera()
    {
        /*
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
        */

        if (window->keyDown(GLFW_KEY_W))
        {
            cameraTransform.moveRelative(glm::vec3(0,-1,0) * deltaTime * cameraSpeed);
        }
        else if (window->keyDown(GLFW_KEY_S))
        {
            cameraTransform.moveRelative(glm::vec3(0,1,0) * deltaTime * cameraSpeed);
        }

        if (window->keyDown(GLFW_KEY_A))
        {
            cameraTransform.moveRelative(glm::vec3(1,0,0) * deltaTime * cameraSpeed);
        }
        else if (window->keyDown(GLFW_KEY_D))
        {
            cameraTransform.moveRelative(glm::vec3(-1,0,0) * deltaTime * cameraSpeed);
        }

        if (window->keyDown(GLFW_KEY_Q))
        {
            cameraBounds += zoomSpeed * deltaTime;
        }
        else if (window->keyDown(GLFW_KEY_E))
        {
            cameraBounds -= zoomSpeed * deltaTime;
        }

/*
        if (window->buttonDown(GLFW_MOUSE_BUTTON_1)){
            glm::vec2 delta = window->mouseDelta;
            cameraTransform.rotate(glm::vec3(delta.y * deltaTime * mouseSpeed, delta.x * deltaTime * mouseSpeed, 0));
        }
        */
    }
} // namespace game
