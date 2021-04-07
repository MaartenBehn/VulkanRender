#include "vulkan.hpp"

namespace game {

Window *window;
const int MAX_FRAMES_IN_FLIGHT = 2;
VkInstance instance;

// Validation
#ifdef DEBUG
const bool enableValidationLayers = true;
#else
const bool enableValidationLayers = false;
#endif
const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
VkDebugUtilsMessengerEXT debugMessenger;
bool checkValidationLayerSupport();
void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

// Surface
VkSurfaceKHR surface;


// Device
VkDevice device;
VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

// SwapChain
struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

VkSwapchainKHR swapChain;
std::vector<VkImage> swapChainImages;
VkFormat swapChainImageFormat;
VkExtent2D swapChainExtent;
std::vector<VkImageView> swapChainImageViews;
std::vector<VkFramebuffer> swapChainFramebuffers;

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

// Renderpass
VkRenderPass renderPass;

// Pipeline
VkPipeline graphicsPipeline;

}  // namespace game