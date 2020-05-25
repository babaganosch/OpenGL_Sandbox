//
//  DirectionalLight.cpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-05-25.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#include "DirectionalLight.hpp"

DirectionalLight::DirectionalLight() : Light()
{
    direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
                                   GLfloat aIntensity, GLfloat dIntensity,
                                   GLfloat xDir, GLfloat yDir, GLfloat zDir) : Light(red, green, blue, aIntensity, dIntensity)
{
    direction = glm::vec3(xDir, yDir, zDir);
}

DirectionalLight::~DirectionalLight() { }

void DirectionalLight::UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation, GLfloat diffuseIntensityLocation, GLfloat directionLocation)
{
    // Ambient
    glUniform3f(ambientColourLocation, colour.r, colour.g, colour.b);
    glUniform1f(ambientIntensityLocation, ambientIntensity);
    // Diffuse
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);
    glUniform3f(directionLocation, direction.x, direction.y, direction.z);
}
