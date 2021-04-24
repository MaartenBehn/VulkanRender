#pragma once

#include "global.hpp"
#include "../../vendor/include/delaunator.hpp"
#include "../../vendor/Perlin_Noise/PerlinNoise.h"

namespace game
{
    void updateIndices()
    {
        mesh.indices.clear();

        std::vector<double> coords;
        for (size_t i = 0; i < particleAmmount; i++)
        {
            coords.push_back(mesh.vertices[i].pos.x);
            coords.push_back(mesh.vertices[i].pos.y);
        }

        delaunator::Delaunator delaunator(coords);

        for (std::size_t i = 0; i < delaunator.triangles.size(); i += 3)
        {
            glm::vec2 p = glm::vec2(
                coords[2 * delaunator.triangles[i]],
                coords[2 * delaunator.triangles[i] + 1]);

            glm::vec2 p1 = glm::vec2(
                coords[2 * delaunator.triangles[i + 1]],
                coords[2 * delaunator.triangles[i + 1] + 1]);

            glm::vec2 p2 = glm::vec2(
                coords[2 * delaunator.triangles[i + 2]],
                coords[2 * delaunator.triangles[i + 2] + 1]);

            float dist = glm::distance(p, p1);
            float dist1 = glm::distance(p1, p2);
            float dist2 = glm::distance(p2, p);

            if (dist > maxParticleDistance || dist1 > maxParticleDistance || dist2 > maxParticleDistance || i > maxTriangles)
            {
                continue;
            }

            mesh.indices.push_back(delaunator.triangles[i]);
            mesh.indices.push_back(delaunator.triangles[i + 1]);
            mesh.indices.push_back(delaunator.triangles[i + 2]);
        }

        while (mesh.indices.size() < maxTriangles * 3)
        {
            mesh.indices.push_back(0);
            mesh.indices.push_back(0);
            mesh.indices.push_back(0);
        }
    }

    void createMesh()
    {
        mesh = Mesh();

        for (size_t i = 0; i < particleAmmount; i++)
        {
            Particle particle = Particle();
            particle.pos = glm::vec2(
                (float)(rand() % bounds),
                (float)(rand() % bounds));
            particle.velocity = glm::vec2(0,0);

            particles.push_back(particle);

            Vertex vertex = Vertex();
            vertex.pos = particle.pos;
            vertex.color = glm::vec3(
                (float)(rand() % 255) / 255,
                (float)(rand() % 255) / 255,
                (float)(rand() % 255) / 255);

            mesh.vertices.push_back(vertex);
        }

        updateIndices();

        VkDeviceSize bufferSize = sizeof(mesh.vertices[0]) * mesh.vertices.size();
        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     vertexBuffer, vertexBufferMemory);
        vkMapMemory(device, vertexBufferMemory, 0, bufferSize, 0, &vertexBufferdata);

        bufferSize = sizeof(mesh.indices[0]) * maxTriangles * 3;
        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     indexBuffer, indexBufferMemory);
        vkMapMemory(device, indexBufferMemory, 0, bufferSize, 0, &indexBufferdata);
    }

    void updateVetices()
    {
        for (size_t i = 0; i < particles.size(); i++)
        {
            glm::vec2 forceVector = glm::vec2(0, 0);
            for (size_t j = 0; j < particles.size(); j++)
            {
                if (i == j)
                {
                    continue;
                }

                glm::vec2 diff = particles[j].pos - particles[i].pos;
                float dist = glm::length(diff);

                if (dist == 0)
                {
                    continue;
                }

                glm::vec2 norm = diff / dist;

                float force = g / (dist * dist);
                forceVector = (-norm * force) + forceVector;
            }
            
            particles[i].velocity += forceVector;
            particles[i].pos += particles[i].velocity;

            mesh.vertices[i].pos = particles[i].pos;
        }
    }

    void updateMesh()
    {
        updateVetices();
        updateIndices();

        VkDeviceSize bufferSize = sizeof(mesh.vertices[0]) * mesh.vertices.size();
        memcpy(vertexBufferdata, mesh.vertices.data(), (size_t)bufferSize);

        bufferSize = sizeof(mesh.indices[0]) * mesh.indices.size();
        memcpy(indexBufferdata, mesh.indices.data(), (size_t)bufferSize);
    }

    void cleanUpMesh()
    {
        vkUnmapMemory(device, vertexBufferMemory);
        vkDestroyBuffer(device, vertexBuffer, nullptr);
        vkFreeMemory(device, vertexBufferMemory, nullptr);

        vkUnmapMemory(device, indexBufferMemory);
        vkDestroyBuffer(device, indexBuffer, nullptr);
        vkFreeMemory(device, indexBufferMemory, nullptr);
    }

} // namespace game
