//
//  Window.cpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-06-01.
//  Copyright � 2020 Lars Andersson. All rights reserved.
//
#include "GameHandler.hpp"
#include "CommonValues.hpp"

GameHandler::GameHandler()
{
	player = Player();
	score = 0;
	maxScore = 10;
}

GameHandler::~GameHandler() 
{

}

void GameHandler::Update(Window* window, PointLight* light, GLfloat dt)
{
	player.Update(window, dt);

    glm::vec3 lightPos = light->GetPosition();
    glm::vec3 playerPos = player.GetModelPosition();

    if (playerPos.x < lightPos.x + 1 && playerPos.x > lightPos.x - 1 && playerPos.z > lightPos.z - 1 && playerPos.z < lightPos.z + 1)
    {
        GLfloat newRed = CommonHelper::uniform_randf(0.0f, 1.0f);
        GLfloat newGreen = CommonHelper::uniform_randf(0.0f, 1.0f);
        GLfloat newBlue = CommonHelper::uniform_randf(0.0f, 1.0f);
        glm::vec3 newColour = glm::normalize(glm::vec3(newRed, newGreen, newBlue));
        light->SetColour(newColour);
        

        GLfloat newX = CommonHelper::uniform_randf(-20.0f, 20.0f);
        GLfloat newZ = CommonHelper::uniform_randf(-20.0f, 20.0f);

        light->SetPosition(glm::vec3(newX, lightPos.y, newZ));

        score++;
    }


}

glm::mat4 GameHandler::GetModelMatrix()
{
	return player.GetModelMatrix();
}
