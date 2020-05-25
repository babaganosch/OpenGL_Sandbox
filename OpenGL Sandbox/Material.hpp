//
//  Material.hpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-05-24.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#ifndef Material_hpp
#define Material_hpp

#include <stdio.h>
#include <GL/glew.h>

class Material
{
public:
    Material();
    Material(GLfloat sIntensity, GLfloat shine);
    ~Material();
    
    void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);
    
private:
    GLfloat specularIntensity;
    GLfloat shininess;
    
};

#endif /* Material_hpp */
