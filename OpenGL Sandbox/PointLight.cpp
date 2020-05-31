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

PointLight::PointLight(GLuint shadowWidth, GLuint shadowHeight,
                       GLfloat near, GLfloat far,
                       GLfloat red, GLfloat green, GLfloat blue,
                       GLfloat aIntensity, GLfloat dIntensity,
                       GLfloat xPos, GLfloat yPos, GLfloat zPos,
                       GLfloat con, GLfloat lin, GLfloat exp) :
Light(shadowWidth, shadowHeight, red, green, blue, aIntensity, dIntensity)
{
    position = glm::vec3(xPos, yPos, zPos);
    constant = con;
    linear   = lin;
    exponent = exp;
    
    farPlane = far;
    float aspect = (float)shadowWidth / (float)shadowHeight;
    lightProj = glm::perspective(glm::radians(90.0f), aspect, near, farPlane);
    
    delete shadowMap;
    shadowMap = new OmniShadowMap();
    shadowMap->Init(shadowWidth, shadowHeight);
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

std::vector<glm::mat4> PointLight::CalculateLightTransform()
{
    std::vector<glm::mat4> lightTransforms;
    
    // +x, -x
    lightTransforms.push_back(lightProj *
        glm::lookAt(position, position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
    lightTransforms.push_back(lightProj *
        glm::lookAt(position, position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
    
    // +y, -y
    lightTransforms.push_back(lightProj *
        glm::lookAt(position, position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
    lightTransforms.push_back(lightProj *
        glm::lookAt(position, position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
    
    // +z, -z
    lightTransforms.push_back(lightProj *
        glm::lookAt(position, position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
    lightTransforms.push_back(lightProj *
        glm::lookAt(position, position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));
    
    //std::reverse(lightTransforms.begin(), lightTransforms.end());

    return lightTransforms;
}
