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
    position = glm::vec3(3.0f, -1.5f, 4.0f);
    scale = glm::vec3(0.005f, 0.005f, 0.005f);
    angle = 0.0f;
}

Player::~Player()
{
    
}

void Player::Update(Window* window, GLfloat dt)
{
    if (window->getKeys()[GLFW_KEY_RIGHT])
    {
        angle += dt * 20.f;
    }
    if (window->getKeys()[GLFW_KEY_LEFT])
    {
        angle -= dt * 20.f;
    }
}

glm::mat4 Player::GetModelMatrix()
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::scale(model, scale);
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    return model;
}
