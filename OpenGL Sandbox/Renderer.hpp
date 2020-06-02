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
    void Clear();
    
    GLuint GetTexture() { return colorTexture; }
    
private:
    GLuint FBO, RBO;
    GLuint colorTexture;
    int width;
    int height;
};

#endif /* Renderer_hpp */
