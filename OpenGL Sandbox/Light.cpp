//
//  Light.cpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-05-24.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#include "Light.hpp"

Light::Light()
{
    colour = glm::vec3(1.0f, 1.0f, 1.0f);
    ambientIntensity = 1.0f;
    
    direction = glm::vec3(0.0f, -1.0f, 0.0f);
    diffuseIntensity = 0.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity)
{
    colour = glm::vec3(red, green, blue);
    ambientIntensity = aIntensity;
    direction = glm::vec3(xDir, yDir, zDir);
    diffuseIntensity = dIntensity;
}

Light::~Light() { }

void Light::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation, GLfloat diffuseIntensityLocation, GLfloat directionLocation)
{
    // Ambient
    glUniform3f(ambientColourLocation, colour.r, colour.g, colour.b);
    glUniform1f(ambientIntensityLocation, ambientIntensity);
    // Diffuse
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);
    glUniform3f(directionLocation, direction.x, direction.y, direction.z);
}
