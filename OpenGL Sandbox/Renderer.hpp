//
//  Renderer.hpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-05-29.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#ifndef Renderer_hpp
#define Renderer_hpp

#include <stdio.h>
#include <GL/glew.h>

class Renderer
{
public:
    Renderer();
    ~Renderer();
    
    void Init(unsigned int width, unsigned int height);
    void RenderToTexture();
    void Reset();
    
    GLuint GetTexture() { return colorTexture; }
    GLuint GetDepthTexture() { return depthTexture; }
    
private:
    GLuint FBO, RBO;
    GLuint colorTexture, depthTexture;
    int width;
    int height;
    int bitDepth;
};

#endif /* Renderer_hpp */
