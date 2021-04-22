#pragma

#include <vector>
#include <glm/glm.hpp>
#include "../../vendor/spheres/src/spheres.cpp"

namespace game
{
    
    
} // namespace game

game::Mesh genSphere(float radius)
{
    Mesh mesh;
    Icosahedron(mesh);

    game::Mesh gameMesh = game::Mesh();
    gameMesh.vertices = std::vector<game::Vertex>(mesh.vertices.size());
    for (size_t i = 0; i < mesh.vertices.size(); i++)
    {
        gameMesh.vertices[i] = game::Vertex();
        gameMesh.vertices[i].pos = glm::vec3(mesh.vertices[i].x, mesh.vertices[i].y, mesh.vertices[i].z);
        gameMesh.vertices[i].color = glm::vec3(
            ((float) rand() / (RAND_MAX)),
            ((float) rand() / (RAND_MAX)),
            ((float) rand() / (RAND_MAX)));
    }

    gameMesh.indices = std::vector<uint16_t>(mesh.triangles.size());
    for (size_t i = 0; i < mesh.triangles.size(); i++)
    {
        gameMesh.indices[i] = mesh.triangles[i];
    }
    
    return gameMesh;
}
