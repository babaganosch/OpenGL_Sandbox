//
//  SpotLight.hpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-05-25.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#ifndef SpotLight_hpp
#define SpotLight_hpp

#include <stdio.h>
#include "PointLight.hpp"

class SpotLight : public PointLight
{
public:
    SpotLight();
    SpotLight(GLuint shadowWidth, GLuint shadowHeight,
              GLfloat near, GLfloat far,
              GLfloat red, GLfloat green, GLfloat blue,
              GLfloat aIntensity, GLfloat dIntensity,
              GLfloat xPos, GLfloat yPos, GLfloat zPos,
              GLfloat xDir, GLfloat yDir, GLfloat zDir,
              GLfloat con, GLfloat lin, GLfloat exp,
              GLfloat edg);
    ~SpotLight();
    
    void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
                  GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
                  GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
                  GLuint edgeLocation);
    
    void SetFlash(glm::vec3 pos, glm::vec3 dir);
    
private:
    glm::vec3 direction;
    
    GLfloat edge, procEdge;
};

#endif /* SpotLight_hpp */
