#pragma once

#include "global.hpp"

namespace game
{
    bool hasExtension(std::string name, std::vector<VkExtensionProperties> *extensions)
    {
        bool found = false;

        for (const auto &extension : *extensions)
        {
            std::string extensionName = extension.extensionName;
            if (name == extensionName)
            {
                found = true;
                break;
            }
        }

        return found;
    }
    std::vector<const char *> getRequiredExtensions()
    {
        // Getting all possible Extensions
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> possibleExtensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, possibleExtensions.data());

        // Getting required GLFW extensions
        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        // Checking if extensions are present
        for (int i = 0; i < glfwExtensionCount; ++i)
        {
            if (!hasExtension(glfwExtensions[i], &possibleExtensions))
            {
                std::cout << '\t' << " GLFW extension: " << glfwExtensions[i]
                          << " not found!" << '\n';
            }
        }

        std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        // Debug Extension
        if (enableValidationLayers)
        {
            if (!hasExtension(VK_EXT_DEBUG_UTILS_EXTENSION_NAME, &possibleExtensions))
            {
                std::cout << '\t' << " Debug extension: " << VK_EXT_DEBUG_UTILS_EXTENSION_NAME
                          << " not found!" << '\n';
            }

            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }

    void createInstance()
    {
        // Setting app info struct
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        // Setting create info struct
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        // Get extensions
        auto extensions = getRequiredExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        // Checking if validation layers are avalable
        if (enableValidationLayers && !checkValidationLayerSupport())
        {
            throw std::runtime_error(
                "validation layers requested, but not available!");
        }

        // Adding validation layers to create info
        if (enableValidationLayers)
        {
            createInfo.enabledLayerCount =
                static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }

        // Debug Messager
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
        if (enableValidationLayers)
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();

            populateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
        }
        else
        {
            createInfo.enabledLayerCount = 0;

            createInfo.pNext = nullptr;
        }

        // Create Instance
        VK_CHECK_RESULT(vkCreateInstance(&createInfo, nullptr, &instance));
    }
} // namespace game
