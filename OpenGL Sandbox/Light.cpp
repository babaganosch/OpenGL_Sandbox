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
    diffuseIntensity = 0.0f;
}

Light::Light(GLuint shadowWidth, GLuint shadowHeight, GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity)
{
    shadowMap = new ShadowMap();
    shadowMap->Init(shadowWidth, shadowHeight);
    
    colour = glm::vec3(red, green, blue);
    ambientIntensity = aIntensity;
    diffuseIntensity = dIntensity;
}

Light::~Light() { }
