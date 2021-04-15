namespace game
{

    bool checkDeviceExtensionSupport(VkPhysicalDevice device)
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        for (const auto &extension : availableExtensions)
        {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }
    bool isDeviceSuitable(VkPhysicalDevice device)
    {
        bool extensionsSupported = checkDeviceExtensionSupport(device);

        bool swapChainAdequate = false;
        if (extensionsSupported)
        {
            SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device);
            swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        return extensionsSupported && swapChainAdequate;
    }
    void pickPhysicalDevice()
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0)
        {
            throw std::runtime_error(
                "failed to find GPUs with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        int bestScore = 0;
        for (const auto &device : devices)
        {
            if (!isDeviceSuitable(device))
            {
                continue;
            }
            int score = 0;

            VkPhysicalDeviceProperties props{};
            vkGetPhysicalDeviceProperties(device, &props);
            if (props.deviceType == VkPhysicalDeviceType::VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
            {
                score += 2000;
            }

            // Determine the available device local memory.
            VkPhysicalDeviceMemoryProperties memoryProps{};
            vkGetPhysicalDeviceMemoryProperties(device, &memoryProps);

            auto heapsPointer = memoryProps.memoryHeaps;
            auto heaps = std::vector<VkMemoryHeap>(heapsPointer, heapsPointer + memoryProps.memoryHeapCount);

            for (const auto &heap : heaps)
            {
                if (heap.flags & VkMemoryHeapFlagBits::VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
                {
                    // Device local heap, should be size of total GPU VRAM.
                    //heap.size will be the size of VRAM in bytes. (bigger is better)
                    score += heap.size / 1000000; // byte / MB -> 1000 per GB
                }
            }

            if (score > bestScore)
            {
                bestScore = score;
                physicalDevice = device;
            }
        }

        if (physicalDevice == VK_NULL_HANDLE)
        {
            throw std::runtime_error("failed to find a suitable GPU!");
        }
    }

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device)
    {
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        std::vector<uint32_t> possibleGraphicsFamilies;
        std::vector<uint32_t> possiblePresentingFamilies;
        std::vector<uint32_t> possibleComputingFamilies;
        std::vector<uint32_t> possibleTransferFamilies;

        uint32_t i = 0;
        for (const auto &queueFamily : queueFamilies)
        {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                possibleGraphicsFamilies.push_back(i);
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
            if (presentSupport)
            {
                possiblePresentingFamilies.push_back(i);
            }

            if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
            {
                possibleComputingFamilies.push_back(i);
            }

            if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT)
            {
                possibleTransferFamilies.push_back(i);
            }

            i++;
        }

        QueueFamilyIndices bestIndices;
        int bestDiversity = -1;
        for (const auto graphicsIndex : possibleGraphicsFamilies)
        {
            for (const auto presentingIndex : possiblePresentingFamilies)
            {
                for (const auto computeIndex : possibleComputingFamilies)
                {
                    for (const auto transferIndex : possibleTransferFamilies)
                    {
                        int diversity =
                            (graphicsIndex != presentingIndex) +
                            (graphicsIndex != computeIndex) * 10 +
                            (graphicsIndex != transferIndex) * 2 +
                            (presentingIndex != computeIndex) * 5 +
                            (presentingIndex != transferIndex) * 2 +
                            (computeIndex != transferIndex);

                        if (diversity > bestDiversity)
                        {
                            bestDiversity = diversity;
                            bestIndices = QueueFamilyIndices{graphicsIndex, presentingIndex, computeIndex, transferIndex};
                        }
                    }
                }
            }
        }

        return bestIndices;
    }

    void createLogicalDevice()
    {
        queueFamilyIndices = findQueueFamilies(physicalDevice);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {
            queueFamilyIndices.graphicsFamily,
            queueFamilyIndices.presentFamily,
            queueFamilyIndices.computeFamily,
            queueFamilyIndices.transferFamily};

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

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

        if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) !=
            VK_SUCCESS)
        {
            throw std::runtime_error("failed to create logical device!");
        }

        vkGetDeviceQueue(device, queueFamilyIndices.graphicsFamily, 0, &graphicsQueue);
        vkGetDeviceQueue(device, queueFamilyIndices.presentFamily, 0, &presentQueue);
        vkGetDeviceQueue(device, queueFamilyIndices.computeFamily, 0, &computeQueue);
        vkGetDeviceQueue(device, queueFamilyIndices.transferFamily, 0, &transferQueue);
    }

} // namespace game
