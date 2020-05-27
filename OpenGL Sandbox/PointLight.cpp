//
//  PointLight.cpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-05-25.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#include "PointLight.hpp"

PointLight::PointLight() : Light()
{
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    constant = 1.0f;
    linear   = 0.0f;
    exponent = 0.0f;
}

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue,
           GLfloat aIntensity, GLfloat dIntensity,
           GLfloat xPos, GLfloat yPos, GLfloat zPos,
           GLfloat con, GLfloat lin, GLfloat exp) : Light(1024, 1024, red, green, blue, aIntensity, dIntensity)
{
    position = glm::vec3(xPos, yPos, zPos);
    constant = con;
    linear   = lin;
    exponent = exp;
}

PointLight::~PointLight() { }

void PointLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
                          GLuint diffuseIntensityLocation, GLuint positionLocation,
                          GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation)
{
    // Ambient
    glUniform3f(ambientColourLocation, colour.r, colour.g, colour.b);
    glUniform1f(ambientIntensityLocation, ambientIntensity);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);
    
    glUniform3f(positionLocation, position.x, position.y, position.z);
    glUniform1f(constantLocation, constant);
    glUniform1f(linearLocation, linear);
    glUniform1f(exponentLocation, exponent);
}
