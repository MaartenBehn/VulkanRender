#pragma once

#include "global.hpp"
#include "../../vendor/include/delaunator.hpp"

namespace game
{
    void createMesh()
    {
        mesh = Mesh();

        std::vector<double> coords;
        for (size_t i = 0; i < particleAmmount; i++)
        {
            Vertex vertex = Vertex();

            vertex.pos = glm::vec2(
                (float)(rand() % bounds),
                (float)(rand() % bounds));

            vertex.color = glm::vec3(
                (float)(rand() % 255) / 255,
                (float)(rand() % 255) / 255,
                (float)(rand() % 255) / 255);

            mesh.vertices.push_back(vertex);

            coords.push_back(vertex.pos.x);
            coords.push_back(vertex.pos.y);
        }

        delaunator::Delaunator d(coords);

        for (std::size_t i = 0; i < d.triangles.size(); i += 3)
        {
            glm::vec2 p = glm::vec2(
                d.coords[2 * d.triangles[i]], 
                d.coords[2 * d.triangles[i] + 1]
                );
            
            glm::vec2 p1 = glm::vec2(
                d.coords[2 * d.triangles[i + 1]], 
                d.coords[2 * d.triangles[i + 1] + 1]
                );

            glm::vec2 p2 = glm::vec2(
                d.coords[2 * d.triangles[i + 2]], 
                d.coords[2 * d.triangles[i + 2] + 1]
                );

            float dist = glm::distance(p, p1);
            float dist1 = glm::distance(p1, p2);
            float dist2 = glm::distance(p2, p);

            if (dist > maxParticleDistance || dist1 > maxParticleDistance || dist2 > maxParticleDistance){
                continue;
            }

            mesh.indices.push_back(d.triangles[i]);
            mesh.indices.push_back(d.triangles[i + 1]);
            mesh.indices.push_back(d.triangles[i + 2]);
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
