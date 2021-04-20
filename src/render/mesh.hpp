#pragma once

#include "global.hpp"

namespace game
{
    void createMesh()
    {
        mesh = Mesh();

        Mesh sphereMesh = genSphere(10.0f);

        particles = std::vector<Particle>(sphereMesh.vertices.size());
        for (size_t i = 0; i < sphereMesh.vertices.size(); i++)
        {
            particles[i] = Particle();
            particles[i].pos = sphereMesh.vertices[i].pos;
        }
        
        for (size_t i = 0; i < particles.size(); i++)
        {
            printf("%ld\n",i);

            std::vector<Particle> nearPartiles = std::vector<Particle>();

            Vertex vertex = Vertex();
            vertex.pos = particles[i].pos;
            vertex.color = glm::vec3(
                ((float) rand() / (RAND_MAX)),
                ((float) rand() / (RAND_MAX)),
                ((float) rand() / (RAND_MAX)));

            for (size_t j = 0; j < particles.size(); j++)
            {
                if (i == j){
                    continue;
                }

                float dist = glm::distance(particles[i].pos, particles[j].pos);

                if (dist < maxParticleDistance){
                    nearPartiles.push_back(particles[j]);
                }
            }

            for (size_t j = 0; j < nearPartiles.size(); j++)
            {
                int indexNext = j + 1;
                if (indexNext >= nearPartiles.size()){
                    indexNext = 0;
                }

                Vertex vertex1 = Vertex();
                vertex1.pos = nearPartiles[j].pos;
                vertex1.color = vertex.color;

                Vertex vertex2 = Vertex();
                vertex2.pos = nearPartiles[indexNext].pos;
                vertex2.color = vertex.color;

                mesh.vertices.push_back(vertex);
                mesh.vertices.push_back(vertex1);
                mesh.vertices.push_back(vertex2);

                mesh.indices.push_back(mesh.indices.size());
                mesh.indices.push_back(mesh.indices.size());
                mesh.indices.push_back(mesh.indices.size());

                 printf("j %ld\n", j);
            }
        }
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

    void createIndexBuffer()
    {
        VkDeviceSize bufferSize = sizeof(mesh.indices[0]) * mesh.indices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void *data;
        vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, mesh.indices.data(), (size_t)bufferSize);
        vkUnmapMemory(device, stagingBufferMemory);

        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

        copyBuffer(stagingBuffer, indexBuffer, bufferSize);

        vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingBufferMemory, nullptr);
    }

} // namespace game
