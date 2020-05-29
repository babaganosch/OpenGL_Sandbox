//
//  ShadowMap.hpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-05-26.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#ifndef ShadowMap_hpp
#define ShadowMap_hpp

#include <stdio.h>
#include <GL/glew.h>

class ShadowMap
{
public:
    ShadowMap();
    ~ShadowMap();
    
    virtual bool Init(GLuint width, GLuint height);
    virtual void Write();
    virtual void Read(GLenum textureUnit);
    
    GLuint GetShadowWidth() { return shadowWidth; }
    GLuint GetShadowHeight() { return shadowHeight; }
    GLuint GetTexture() { return shadowMap; }
    
protected:
    GLuint FBO, shadowMap;
    GLuint shadowWidth, shadowHeight;
};

#endif /* ShadowMap_hpp */
