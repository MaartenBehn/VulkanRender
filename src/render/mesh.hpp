#pragma once

#include "global.hpp"

namespace game
{
    void createMesh()
    {
        mesh = Mesh();

        particles = std::vector<Particle>(particleAmmount);
        for (size_t i = 0; i < particleAmmount; i++)
        {
            particles[i] = Particle();
            particles[i].pos = glm::vec3(
                (float)(rand() % bounds),
                (float)(rand() % bounds),
                (float)(rand() % bounds));
            particles[i].id = i;
                
            printf("%ld\r", i);

            Vertex vertex = Vertex();
            vertex.pos = particles[i].pos;
            vertex.color = glm::vec3(
                (float)(rand() % 255) / 255,
                (float)(rand() % 255) / 255,
                (float)(rand() % 255) / 255);

            mesh.vertices.push_back(vertex);
        }

        for (size_t i = 0; i < particles.size(); i++)
        {
            printf("%ld\r", i);
            std::vector<Particle> nearPartiles = std::vector<Particle>();

            for (size_t j = 0; j < particles.size(); j++)
            {
                float dist = glm::distance(particles[i].pos, particles[j].pos);

                if (dist < maxParticleDistance)
                {
                    nearPartiles.push_back(particles[j]);
                }
            }

            if (nearPartiles.size() < minNearParticles)
            {

                continue;
            }

            for (size_t j = 0; j < nearPartiles.size(); j++)
            {
                for (size_t j1 = 0; j1 < nearPartiles.size(); j1++)
                {
                    for (size_t j2 = 0; j2 < nearPartiles.size(); j2++)
                    {

                        if (j == j1 || j == j2 || j1 == j2)
                        {
                            continue;
                        }

                        float x = nearPartiles[j].pos.x;
                        float x1 = nearPartiles[j1].pos.x;
                        float x2 = nearPartiles[j2].pos.x;

                        if (x > x1 || x > x2)
                        {
                            continue;
                        }

                        mesh.indices.push_back(nearPartiles[j].id);
                        mesh.indices.push_back(nearPartiles[j1].id);
                        mesh.indices.push_back(nearPartiles[j2].id);
                    }
                }
            }
        }

        printf("%ld\n", mesh.vertices.size());
        printf("%ld\n", mesh.indices.size());
    }

    void createVertexBuffer()
    {
        VkDeviceSize bufferSize = sizeof(mesh.vertices[0]) * mesh.vertices.size();

        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     vertexBuffer, vertexBufferMemory);

        vkMapMemory(device, vertexBufferMemory, 0, bufferSize, 0, &vertexBufferdata);
    }

    void updateVertexBuffer()
    {
        VkDeviceSize bufferSize = sizeof(mesh.vertices[0]) * mesh.vertices.size();

        memcpy(vertexBufferdata, mesh.vertices.data(), (size_t)bufferSize);
    }

    void cleanUpVertexBuffer()
    {
        vkUnmapMemory(device, vertexBufferMemory);
        vkDestroyBuffer(device, vertexBuffer, nullptr);
        vkFreeMemory(device, vertexBufferMemory, nullptr);
    }

    void createIndexBuffer()
    {
        VkDeviceSize bufferSize = sizeof(mesh.indices[0]) * mesh.indices.size();

        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     indexBuffer, indexBufferMemory);

        vkMapMemory(device, indexBufferMemory, 0, bufferSize, 0, &indexBufferdata);
    }

    void updateIndexBuffer()
    {
        VkDeviceSize bufferSize = sizeof(mesh.indices[0]) * mesh.indices.size();

        memcpy(indexBufferdata, mesh.indices.data(), (size_t)bufferSize);
    }

    void cleanUpIndexBuffer()
    {
        vkUnmapMemory(device, indexBufferMemory);
        vkDestroyBuffer(device, indexBuffer, nullptr);
        vkFreeMemory(device, indexBufferMemory, nullptr);
    }

} // namespace game
