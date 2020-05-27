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
#include <vector>
#include "Light.hpp"
#include "OmniShadowMap.hpp"

class PointLight : public Light
{
public:
    PointLight();
    PointLight(GLuint shadowWidth, GLuint shadowHeight,
               GLfloat near, GLfloat far,
               GLfloat red, GLfloat green, GLfloat blue,
               GLfloat aIntensity, GLfloat dIntensity,
               GLfloat xPos, GLfloat yPos, GLfloat zPos,
               GLfloat con, GLfloat lin, GLfloat exp);
    ~PointLight();
    
    void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
                  GLuint diffuseIntensityLocation, GLuint positionLocation,
                  GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);
    
    std::vector<glm::mat4> CalculateLightTransform();
    
    GLfloat GetFarPlane() { return farPlane; }
    glm::vec3 GetPosition() { return position; }
    
protected:
    glm::vec3 position;
    
    GLfloat constant, linear, exponent;
    GLfloat farPlane;
};

#endif /* PointLight_hpp */
