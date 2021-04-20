#pragma once

#include <iostream>
#include <cstdio>

#include "gui.hpp"

namespace game
{
    class Transform
    {
    public:
        Transform();

        glm::vec3 getPos();
        void setPos(glm::vec3);
        void moveRelative(glm::vec3);

        glm::vec3 getRot();
        void setRot(glm::vec3);
        void rotate(glm::vec3);

        glm::mat4 getMat();

        ~Transform();

    private:
        glm::vec3 position;
        glm::vec3 rotation;

        glm::mat4 rotationMatrix;
        bool needsRotateMatrixUpdate;

        glm::mat4 matrix;
        bool needsMatrixUpdate;

        void updateRotationMatrix();
        void updateMatrix();
    };

    Transform::Transform()
    {
        position = glm::vec3(0, 0, 0);
        position = glm::vec3(0, 0, 0);

        needsRotateMatrixUpdate = true;
        needsMatrixUpdate = true;
    }

    void Transform::updateRotationMatrix()
    {
        if (needsRotateMatrixUpdate)
        {
            rotationMatrix = glm::mat4(1);
            rotationMatrix *= glm::rotate(rotationMatrix, rotation.x, glm::vec3(1, 0, 0));
            rotationMatrix *= glm::rotate(rotationMatrix, rotation.y, glm::vec3(0, 1, 0));
            rotationMatrix *= glm::rotate(rotationMatrix, rotation.z, glm::vec3(0, 0, 1));

            needsRotateMatrixUpdate = false;
        }
    }

    void Transform::updateMatrix()
    {
        updateRotationMatrix();

        if (needsMatrixUpdate)
        {
            matrix = glm::mat4(1);
            matrix = glm::translate(matrix, position);

            matrix = rotationMatrix * matrix;

            needsMatrixUpdate = false;
        }
    }

    glm::vec3 Transform::getPos()
    {
        return position;
    }

    void Transform::setPos(glm::vec3 pos)
    {
        position = pos;

        needsMatrixUpdate = true;
        updateMatrix();
    }

    void Transform::moveRelative(glm::vec3 vec){
        
        if (vec.x != 0 || vec.y != 0 || vec.z != 0){
            updateRotationMatrix();
            glm::vec4 vec4 = glm::vec4(vec, 0);
            vec = vec4 * rotationMatrix;
        }
        
        position += vec;

        needsMatrixUpdate = true;
        updateMatrix();
    }

    glm::vec3 Transform::getRot()
    {
        return glm::degrees(rotation);
    }

    void Transform::setRot(glm::vec3 rot)
    {
        rotation = glm::radians(rot);

        needsRotateMatrixUpdate = true;
        needsMatrixUpdate = true;
        updateMatrix();
    }

    void Transform::rotate(glm::vec3 rot)
    {
        rotation += glm::radians(rot);

        needsRotateMatrixUpdate = true;
        needsMatrixUpdate = true;
        updateMatrix();
    }

    glm::mat4 Transform::getMat()
    {
        return matrix;
    }

    Transform::~Transform()
    {
    }

} // namespace game
