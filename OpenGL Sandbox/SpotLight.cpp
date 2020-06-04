//
//  SpotLight.cpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-05-25.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#include "SpotLight.hpp"

SpotLight::SpotLight() : PointLight()
{
    direction = glm::vec3(0.0f, -1.0f, 0.0f);
    edge = 0;
    procEdge = cosf(glm::radians(edge));
    isOn = false;
}

SpotLight::SpotLight(GLuint shadowWidth, GLuint shadowHeight,
                     GLfloat near, GLfloat far,
                     GLfloat red, GLfloat green, GLfloat blue,
                     GLfloat aIntensity, GLfloat dIntensity,
                     GLfloat xPos, GLfloat yPos, GLfloat zPos,
                     GLfloat xDir, GLfloat yDir, GLfloat zDir,
                     GLfloat con, GLfloat lin, GLfloat exp,
                     GLfloat edg) :
PointLight(shadowWidth, shadowHeight, near, far, red, green, blue, aIntensity, dIntensity, xPos, yPos, zPos, con, lin, exp)
{
    direction = glm::normalize(glm::vec3(xDir, yDir, zDir));
    edge = edg;
    procEdge = cosf(glm::radians(edge));
    isOn = false;
}

SpotLight::~SpotLight()
{
    
}

void SpotLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
              GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
              GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
              GLuint edgeLocation)
{
    glUniform3f(ambientColourLocation, colour.r, colour.g, colour.b);
    
    if (isOn)
    {
        glUniform1f(ambientIntensityLocation, ambientIntensity);
        glUniform1f(diffuseIntensityLocation, diffuseIntensity);
    } else {
        glUniform1f(ambientIntensityLocation, 0.0f);
        glUniform1f(diffuseIntensityLocation, 0.0f);
    }
    
    glUniform3f(positionLocation, position.x, position.y, position.z);
    glUniform1f(constantLocation, constant);
    glUniform1f(linearLocation, linear);
    glUniform1f(exponentLocation, exponent);
    
    glUniform3f(directionLocation, direction.x, direction.y, direction.z);
    glUniform1f(edgeLocation, procEdge);
}

void SpotLight::SetFlash(glm::vec3 pos, glm::vec3 dir)
{
    position = pos;
    direction = dir;
}
