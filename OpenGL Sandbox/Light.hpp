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
#include <GL/glew.h>

class Light
{
public:
    Light();
    Light(GLfloat red, GLfloat green, GLfloat blue,
          GLfloat aIntensity, GLfloat dIntensity);
    ~Light();
    
protected:
    glm::vec3 colour;
    GLfloat ambientIntensity;
    GLfloat diffuseIntensity;
};

#endif /* Light_hpp */
