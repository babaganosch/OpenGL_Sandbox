//
//  GBuffer.cpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-06-01.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#include "GBuffer.hpp"

GBuffer::GBuffer()
{
    width = 0;
    height = 0;
    FBO = 0;
    diffuseRT = 0;
    positionRT = 0;
    normalsRT = 0;
    depthRT = 0;
    diffuseTexture = 0;
    positionTexture = 0;
    normalsTexture = 0;
    depthTexture = 0;
    
    shader = nullptr;
    uniformProjection = 0;
    uniformView = 0;
    uniformTexture = 0;
}

GBuffer::~GBuffer()
{
    glDeleteTextures(1, &depthTexture);
    glDeleteTextures(1, &normalsTexture);
    glDeleteTextures(1, &positionTexture);
    glDeleteTextures(1, &diffuseTexture);
    glDeleteFramebuffers(1, &FBO);
    glDeleteRenderbuffers(1, &diffuseRT);
    glDeleteRenderbuffers(1, &positionRT);
    glDeleteRenderbuffers(1, &normalsRT);
    glDeleteRenderbuffers(1, &depthRT);
}

void GBuffer::Init(GLuint w, GLuint h, Shader* sh)
{
    width = w;
    height = h;
    shader = sh;
    
    uniformProjection = shader->GetProjectionLocation();
    uniformView = shader->GetViewLocation();
    uniformTexture = shader->GetTheTextureLocation();
    
    glGenFramebuffers(1, &FBO);
    glGenRenderbuffers(1, &diffuseRT);
    glGenRenderbuffers(1, &positionRT);
    glGenRenderbuffers(1, &normalsRT);
    glGenRenderbuffers(1, &depthRT);
    
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    
    // Diffuse render target
    glBindRenderbuffer(GL_RENDERBUFFER, diffuseRT);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, diffuseRT);
    
    // Position render target
    glBindRenderbuffer(GL_RENDERBUFFER, positionRT);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA32F, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_RENDERBUFFER, positionRT);
    
    // Normals render target
    glBindRenderbuffer(GL_RENDERBUFFER, normalsRT);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA16F, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_RENDERBUFFER, normalsRT);
    
    // Depth render target
    glBindRenderbuffer(GL_RENDERBUFFER, depthRT);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRT);
    
    // Generate and attach diffuse texture
    glGenTextures(1, &diffuseTexture);
    glBindTexture(GL_TEXTURE_2D, diffuseTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, diffuseTexture, 0);
    
    // Generate and attach position texture
    glGenTextures(1, &positionTexture);
    glBindTexture(GL_TEXTURE_2D, positionTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, positionTexture, 0);
    
    // Generate and attach normals texture
    glGenTextures(1, &normalsTexture);
    glBindTexture(GL_TEXTURE_2D, normalsTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, normalsTexture, 0);
    
    // Generate and attach depth texture
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
    
    GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, buffers);
    
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("GBuffer - Framebuffer Error: %i\n", status);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        return;
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBuffer::Render(glm::mat4* projection, glm::mat4* viewMatrix)
{
    shader->UseShader();
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, value_ptr(*projection));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, value_ptr(*viewMatrix));
    glUniform1i(uniformTexture, 1);
    
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glViewport(0, 0, width, height);
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
}

void GBuffer::Reset()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUseProgram(0);
}
