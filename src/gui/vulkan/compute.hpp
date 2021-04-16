#include "png/lodepng.h"

namespace game
{
    void saveRenderedImage() {
        void* mappedMemory = NULL;
        // Map the buffer memory, so that we can read from it on the CPU.
        vkMapMemory(device, computeBufferMemory, 0, computeBufferSize, 0, &mappedMemory);
        Pixel* pmappedMemory = (Pixel *)mappedMemory;

        // Get the color data from the buffer, and cast it to bytes.
        // We save the data to a vector.
        std::vector<unsigned char> image;
        image.reserve( computeWith * computeHeight * 4);
        for (int i = 0; i < computeWith*computeHeight; i += 1) {
            image.push_back((unsigned char)(255.0f * (pmappedMemory[i].r)));
            image.push_back((unsigned char)(255.0f * (pmappedMemory[i].g)));
            image.push_back((unsigned char)(255.0f * (pmappedMemory[i].b)));
            image.push_back((unsigned char)(255.0f * (pmappedMemory[i].a)));
        }
        // Done reading, so unmap.
        vkUnmapMemory(device, computeBufferMemory);

        // Now we save the acquired color data to a .png.
        unsigned error = lodepng::encode("mandelbrot.png", image, computeWith, computeHeight);
        if (error) printf("encoder error %d: %s", error, lodepng_error_text(error));
        
    }

    void createComputeBuffer(){
        createBuffer(computeBufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, 
        computeBuffer, computeBufferMemory);
    }
} // namespace game
