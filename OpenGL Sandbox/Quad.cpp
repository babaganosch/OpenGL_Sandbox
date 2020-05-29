//
//  Quad.cpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-05-28.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#include "Quad.hpp"

Quad::Quad()
{
    VAO = 0;
    VBO = 0;
}

Quad::~Quad()
{
    
}

void Quad::Init()
{

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
        static const GLfloat vao_data[] =
        {
            -1.0f, 1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            
            -1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f
        };
    
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
            glBufferData(GL_ARRAY_BUFFER, sizeof(vao_data), vao_data, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2* sizeof(GLfloat)));
    
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
}

void Quad::RenderFB()
{
    glBindVertexArray( VAO );
    glDisable( GL_DEPTH_TEST );
    glDrawArrays( GL_TRIANGLES, 0, 6 );
    glEnable( GL_DEPTH_TEST );
    glBindVertexArray(0);
}

void Quad::RenderTexture( GLuint texture )
{
    glBindVertexArray( VAO );
    glDisable( GL_DEPTH_TEST );
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, texture );
    glDrawArrays( GL_TRIANGLES, 0, 6 );
    glEnable( GL_DEPTH_TEST );
    glBindVertexArray(0);
}
