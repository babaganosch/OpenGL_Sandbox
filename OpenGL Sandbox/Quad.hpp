//
//  Quad.hpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-05-28.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#ifndef Quad_hpp
#define Quad_hpp

#include <stdio.h>
#include <GL/glew.h>

class Quad
{
public:
    Quad();
    ~Quad();
    
    void Init(unsigned int width, unsigned int height);
    void RenderToTexture();
    void RenderTextureToScreen();
    
private:
    GLuint FBO, RBO, VAO, VBO;
    GLuint textureID;
    
    int width;
    int height;
    int bitDepth;
};

#endif /* Quad_hpp */
