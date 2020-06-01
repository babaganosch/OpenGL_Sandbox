//
//  Player.cpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-06-01.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#include "Player.hpp"

Player::Player()
{
    model = glm::mat4(1.0f);
    angle = 0.0f;
    maxSpeed = 20.f;
    currentSpeed = 0.0f;
    rotateSpeed = 0.07f;
    acceleration = 15.f;

    glm::mat4 R = glm::rotate(glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 T = R  * glm::translate(glm::vec3(3.0f, -2.0f, 4.0f)) * glm::scale(glm::vec3(0.005f, 0.005f, 0.005f));

    model *= T;
}

Player::~Player()
{
    
}

void Player::Update(Window* window, GLfloat dt)
{
    if (window->getKeys()[GLFW_KEY_UP])
    {
        currentSpeed -= dt * acceleration;
    }
    if (window->getKeys()[GLFW_KEY_DOWN])
    {
        currentSpeed += dt * acceleration;
    }
    if (window->getKeys()[GLFW_KEY_RIGHT])
    {
        angle = currentSpeed * rotateSpeed;
    }
    if (window->getKeys()[GLFW_KEY_LEFT])
    {
        angle = - currentSpeed * rotateSpeed;
    }
   
    currentSpeed = glm::clamp(currentSpeed, -maxSpeed, maxSpeed);

    glm::mat4 R = glm::rotate(glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 T = R * glm::translate(glm::vec3(0.0f, 0.0f, currentSpeed));

    model *= T;

    if (currentSpeed > dt * acceleration * 0.5) {
        currentSpeed -= dt * acceleration * 0.5;
    }
    else if (currentSpeed < -dt * acceleration * 0.5) {
        currentSpeed += dt * acceleration * 0.5;
    }
    else {
        currentSpeed = 0.f;
    }
    angle = 0;
}

glm::mat4 Player::GetModelMatrix()
{
    return model;
}
glm::vec3 Player::GetModelPosition()
{
    return glm::vec3(model[3]);
}
