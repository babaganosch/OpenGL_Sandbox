//
//  Mesh.hpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-05-19.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include <stdio.h>
#include <GL/glew.h>

#include "Shader.hpp"

class Mesh
{
public:
    Mesh();
    ~Mesh();
    
    void CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices);
    void RenderMesh();
    void ClearMesh();
    GLuint GetVAO();
    
private:
    GLuint VAO, VBO, IBO;
    GLsizei indexCount;
};

#endif /* Mesh_hpp */
