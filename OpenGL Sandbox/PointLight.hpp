//
//  PointLight.hpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-05-25.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#ifndef PointLight_hpp
#define PointLight_hpp

#include <stdio.h>
#include "Light.hpp"

class PointLight : public Light
{
public:
    PointLight();
    PointLight(GLfloat red, GLfloat green, GLfloat blue,
               GLfloat aIntensity, GLfloat dIntensity,
               GLfloat xPos, GLfloat yPos, GLfloat zPos,
               GLfloat con, GLfloat lin, GLfloat exp);
    ~PointLight();
    
    void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
                  GLuint diffuseIntensityLocation, GLuint positionLocation,
                  GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);
    
protected:
    glm::vec3 position;
    
    GLfloat constant, linear, exponent;
};

#endif /* PointLight_hpp */
