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
    
    void Init();
    void RenderTexture( GLuint texture );
    void RenderFB();
    
private:
    GLuint VAO, VBO;
};

#endif /* Quad_hpp */
