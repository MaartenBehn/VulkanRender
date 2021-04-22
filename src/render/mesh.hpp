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
            printf("%ld\r", i);
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

                        Vertex vertex = Vertex();
                        vertex.pos = nearPartiles[j].pos;
                        vertex.color = glm::vec3(
                            (float)(rand() % 255) / 255,
                            (float)(rand() % 255) / 255,
                            (float)(rand() % 255) / 255);

                        Vertex vertex1 = Vertex();
                        vertex1.pos = nearPartiles[j1].pos;
                        vertex1.color = vertex.color;

                        Vertex vertex2 = Vertex();
                        vertex2.pos = nearPartiles[j2].pos;
                        vertex2.color = vertex.color;

                        mesh.vertices.push_back(vertex);
                        mesh.vertices.push_back(vertex1);
                        mesh.vertices.push_back(vertex2);
                    }
                }
            }
        }

        printf("%ld\n", mesh.vertices.size());
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

        //mesh.vertices[0].pos = glm::vec3(0.0f, 0.01f, 0) + mesh.vertices[0].pos;
        memcpy(vertexBufferdata, mesh.vertices.data(), (size_t)bufferSize);
    }

    void cleanUpVertexBuffer()
    {
        vkUnmapMemory(device, vertexBufferMemory);
        vkDestroyBuffer(device, vertexBuffer, nullptr);
        vkFreeMemory(device, vertexBufferMemory, nullptr);
    }
} // namespace game
