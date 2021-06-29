#pragma once

#include "global.hpp"
#include "../../vendor/png/lodepng.h"

namespace game
{
    void computeSaveRenderedImage()
    {
        void *mappedMemory = NULL;
        // Map the buffer memory, so that we can read from it on the CPU.
        vkMapMemory(device, computeBufferMemory, 0, computeBufferSize, 0, &mappedMemory);
        Pixel *pmappedMemory = (Pixel *)mappedMemory;

        // Get the color data from the buffer, and cast it to bytes.
        // We save the data to a vector.
        std::vector<unsigned char> image;
        image.reserve(WIDTH * HEIGHT * 4);
        for (int i = 0; i < WIDTH * HEIGHT; i += 1)
        {
            image.push_back((unsigned char)(255.0f * (pmappedMemory[i].r)));
            image.push_back((unsigned char)(255.0f * (pmappedMemory[i].g)));
            image.push_back((unsigned char)(255.0f * (pmappedMemory[i].b)));
            image.push_back((unsigned char)(255.0f * (pmappedMemory[i].a)));
        }
        // Done reading, so unmap.
        vkUnmapMemory(device, computeBufferMemory);

        // Now we save the acquired color data to a .png.
        unsigned error = lodepng::encode("mandelbrot.png", image, WIDTH, HEIGHT);
        if (error)
            printf("encoder error %d: %s", error, lodepng_error_text(error));
    }

    void createComputeBuffer()
    {
        createBuffer(computeBufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, 
            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, computeBuffer, computeBufferMemory);
    }

    void runComputeCommandBuffer()
    {
        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;                  
        submitInfo.pCommandBuffers = &computeCommandBuffer; 
       
        VK_CHECK_RESULT(vkQueueSubmit(computeQueue, 1, &submitInfo, computeFence));
       
        VK_CHECK_RESULT(vkWaitForFences(device, 1, &computeFence, VK_TRUE, 100000000000));
    }

    void cleanupCompute()
    {
        vkDestroyFence(device, computeFence, NULL);
        vkFreeMemory(device, computeBufferMemory, NULL);
        vkDestroyBuffer(device, computeBuffer, NULL);
        vkDestroyDescriptorPool(device, computeDescriptorPool, NULL);
        vkDestroyDescriptorSetLayout(device, computeDescriptorSetLayout, NULL);
        vkDestroyPipelineLayout(device, computePipelineLayout, NULL);
        vkDestroyPipeline(device, computePipeline, NULL);
        vkDestroyCommandPool(device, computeCommandPool, NULL);
    }
} // namespace game
