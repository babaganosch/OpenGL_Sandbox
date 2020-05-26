//
//  Light.hpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-05-24.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#ifndef Light_hpp
#define Light_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

#include "ShadowMap.hpp"

class Light
{
public:
    Light();
    Light(GLfloat shadowWidth, GLfloat shadowHeight,
          GLfloat red, GLfloat green, GLfloat blue,
          GLfloat aIntensity, GLfloat dIntensity);
    ~Light();
    
    ShadowMap* GetShadowMap() { return shadowMap; }
    
protected:
    glm::vec3 colour;
    GLfloat ambientIntensity;
    GLfloat diffuseIntensity;
    
    glm::mat4 lightProj;
    
    ShadowMap* shadowMap;
};

#endif /* Light_hpp */
