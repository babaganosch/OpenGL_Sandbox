//
//  Mesh.cpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-05-19.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#include "Mesh.hpp"

Mesh::Mesh()
{
    VAO = 0;
    VBO = 0;
    IBO = 0;
    indexCount = 0;
}

void Mesh::CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices)
{
    indexCount = numOfIndices;
    
    // 1 array, store @ VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
        glGenBuffers(1, &IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);
        
            // 1 buffer, store @ VBO
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW); // target, size, data, usage
                
                // Location 0 = vCoordinates
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0); // location, size, type, normalize values, stride, offset
                glEnableVertexAttribArray(0); // Enable location 0
                // Location 1 = UV coordinates
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3));
                glEnableVertexAttribArray(1);
                // Location 2 = Normals
                glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5));
                glEnableVertexAttribArray(2);
    
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
}

void Mesh::RenderMesh()
{
    if (VAO == 0) return;
    glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
            glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::ClearMesh()
{
    if (IBO != 0)
    {
        glDeleteBuffers(1, &IBO);
        IBO = 0;
    }
    
    if (VBO != 0)
    {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }
    
    if (VAO != 0)
    {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }
    
    indexCount = 0;
}

GLuint Mesh::GetVAO() {
    return VAO;
}


Mesh::~Mesh()
{
    ClearMesh();
}
