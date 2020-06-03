//
//  Camera.cpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-05-23.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#include "Camera.hpp"

Camera::Camera()
{
    Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);
    followingTarget = false;
}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
    position = startPosition;
    worldUp = startUp;
    yaw = startYaw;
    pitch = startPitch;
    front = glm::vec3(1.0f, 0.0f, 0.0f);
    moveSpeed = startMoveSpeed;
    turnSpeed = startTurnSpeed;
    
    followingTarget = false;
    
    update();
}

Camera::~Camera()
{
    
}

void Camera::setTarget(Player* pl) {
    if (followingTarget) {
        followingTarget = false;
    } else {
        followingTarget = true;
        target = pl;
    }
}

void Camera::keyControl(GLfloat dt, bool* keys)
{
    GLfloat velocity = moveSpeed * dt;
    
    if (keys[GLFW_KEY_W])
    {
        position += front * velocity;
    }
    if (keys[GLFW_KEY_S])
    {
        position -= front * velocity;
    }
    if (keys[GLFW_KEY_A])
    {
        position -= right * velocity;
    }
    if (keys[GLFW_KEY_D])
    {
        position += right * velocity;
    }
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
    
    xChange *= turnSpeed;
    yChange *= turnSpeed;
    
    yaw += xChange;
    yaw = fmodf(yaw, 360.0);
    pitch += yChange;
    pitch = glm::clamp(pitch, -89.0f, 89.0f);
    
    update();
}

glm::mat4 Camera::calculateViewMatrix()
{
    if (followingTarget)
    {
        glm::vec3 targetPosition = target->GetModelPosition();
        return glm::lookAt(position, targetPosition, worldUp);
    } else {
        return glm::lookAt(position, position + front, up);
    }
}

glm::vec3 Camera::getCameraPosition()
{
    return position;
}

glm::vec3 Camera::getCameraDirection()
{
    return glm::normalize(front);
}

void Camera::update()
{
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
    
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
    
}
