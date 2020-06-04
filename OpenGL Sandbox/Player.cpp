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
    drift = 0.0f;

    glm::mat4 R = glm::rotate(glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 T = R * glm::translate(glm::vec3(3.0f, -2.0f, 4.0f)) * glm::scale(glm::vec3(0.005f, 0.005f, 0.005f));

    cameraPos = glm::vec3(0.0f);
    
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
    
    if (window->getGPAvailable())
    {
        GLFWgamepadstate states = window->getGPStates();
        float hAxis = states.axes[GLFW_GAMEPAD_AXIS_LEFT_X];
        float accel = (states.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER] + 1.f) / 2.0f;
        float brk = (states.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER] + 1.f) / 2.0f;
        
        // Horzontal axis
        if (hAxis > 0.1f || hAxis < -0.1f) {
            if (abs(drift) > 1)
            {
                angle = CommonHelper::lerp(angle, currentSpeed * rotateSpeed * hAxis, dt * 3);
            }
            else
            {
                angle = CommonHelper::lerp(angle, currentSpeed * rotateSpeed * hAxis, dt * 7);
            }
        }
        else {
            if (abs(drift) > 1)
            {
                angle = CommonHelper::lerp(angle, 0, dt * 3);
            }
            else
            {
                angle = CommonHelper::lerp(angle, 0, dt * 7);
            }
        }
        // Acceleration
        if (accel > 0.1) {
            currentSpeed -= dt * acceleration * accel;
        }
        // Break
        if (brk > 0.1) {
            currentSpeed += dt * acceleration * brk;
        }
        // Left bumper
        if (states.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER] && abs(angle) > 1 && currentSpeed < 0) // No drifting unless turning some
        {
            drift = CommonHelper::lerp(drift, 20.f * angle, dt * 3.0f);
            rotateSpeed = 0.08;
            maxSpeed = 25.f;
        }
        else if(abs(drift) > 0.001)
        {
            drift = CommonHelper::lerp(drift, 0.0f, dt * 3.0f);
            maxSpeed = 20.f;
            rotateSpeed = 0.07;
        }
    }
   
    //currentSpeed = glm::clamp(currentSpeed, -maxSpeed, maxSpeed);

    glm::mat4 R = glm::rotate(glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 T = R * glm::translate(glm::vec3(0.0f, 0.0f, currentSpeed));

    glm::mat4 Cam = model * glm::translate(glm::vec3(0.0f, 1000.0f, 2000.0f));

    cameraPos = Cam[3];
    
    model *= T;

    if (currentSpeed > maxSpeed)
    {
        currentSpeed -= dt * acceleration * 1.1;
    }
    else if (currentSpeed < -maxSpeed)
    {
        currentSpeed += dt * acceleration * 1.1;
    }
    else if (currentSpeed > dt * acceleration * 0.5)
    {
        currentSpeed -= dt * acceleration * 0.5;
    }
    else if (currentSpeed < -dt * acceleration * 0.5)
    {
        currentSpeed += dt * acceleration * 0.5;
    }
    else {
        currentSpeed = 0.f;
    }
}

glm::mat4 Player::GetModelMatrix()
{
    return model * glm::rotate(glm::radians(drift), glm::vec3(0.0f, 1.0f, 0.0f));
}
glm::vec3 Player::GetModelPosition()
{
    return glm::vec3(model[3]);
}
