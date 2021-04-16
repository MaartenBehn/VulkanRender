
namespace game
{

    Window *window;
    const int MAX_FRAMES_IN_FLIGHT = 2;
    VkInstance instance;

    // Validation
#ifdef DEBUG
    const bool enableValidationLayers = true;
#else
    const bool enableValidationLayers = false;
#endif

#define VK_CHECK_RESULT(f)                                                                \
    {                                                                                     \
        VkResult res = (f);                                                               \
        if (res != VK_SUCCESS)                                                            \
        {                                                                                 \
            printf("Fatal : VkResult is %d in %s at line %d\n", res, __FILE__, __LINE__); \
            assert(res == VK_SUCCESS);                                                    \
        }                                                                                 \
    }

    const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    VkDebugUtilsMessengerEXT debugMessenger;
    bool checkValidationLayerSupport();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

    // Surface
    VkSurfaceKHR surface;

    // Device
    VkDevice device;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    struct QueueFamilyIndices
    {
        uint32_t graphicsFamily;
        uint32_t presentFamily;
        uint32_t computeFamily;
        uint32_t transferFamily;
    };
    QueueFamilyIndices queueFamilyIndices;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkQueue computeQueue;
    VkQueue transferQueue;

    // SwapChain
    struct SwapChainSupportDetails
    {
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

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    // Renderpass
    VkRenderPass renderPass;

    // Graphics Pipeline
    VkPipeline graphicsPipeline;
    VkPipelineLayout graphicsPipelineLayout;

    // Compute Pipeline
    VkPipeline computePipeline;
    VkPipelineLayout computePipelineLayout;
    VkShaderModule computeShaderModule;

    static std::vector<char> readFile(const std::string &filename);

    // Comand
    VkCommandPool graphicsCommandPool;
    std::vector<VkCommandBuffer> graphicsCommandBuffers;

    VkCommandPool computeCommandPool;
    VkCommandBuffer computeCommandBuffer;

    // Descriptor
    VkDescriptorPool graphicsDescriptorPool;
    VkDescriptorSetLayout graphicsDescriptorSetLayout;
    std::vector<VkDescriptorSet> graphicsDescriptorSets;

    VkDescriptorPool computeDescriptorPool;
    VkDescriptorSet cpmouteDescriptorSet;
    VkDescriptorSetLayout computeDescriptorSetLayout;

    struct UniformBufferObject
    {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;

    // Vertex Buffer
    VkBuffer vertexBuffer;

    struct Vertex
    {
        glm::vec2 pos;
        glm::vec3 color;
    };
    const std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}};

    // Index Buffer
    VkBuffer indexBuffer;
    const std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0};

    // Compute Buffer
    struct Pixel {
        float r, g, b, a;
    };

    VkBuffer computeBuffer;
    VkDeviceMemory computeBufferMemory;
        
    uint32_t computeBufferSize; // size of `buffer` in bytes.

    // Sync
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;

} // namespace game