//
//  DirectionalLight.hpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-05-25.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#ifndef DirectionalLight_hpp
#define DirectionalLight_hpp

#include <stdio.h>
#include "Light.hpp"

class DirectionalLight : public Light
{
public:
    DirectionalLight();
    DirectionalLight(GLfloat shadowWidth, GLfloat shadowHeight,
                     GLfloat red, GLfloat green, GLfloat blue,
                     GLfloat aIntensity, GLfloat dIntensity,
                     GLfloat xDir, GLfloat yDir, GLfloat zDir);
    ~DirectionalLight();
    
    glm::mat4 CalculateLightTransform();
    
    void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation, GLfloat diffuseIntensityLocation, GLfloat directionLocation);
    
private:
    glm::vec3 direction;
};

#endif /* DirectionalLight_hpp */
