//
//  GBuffer.hpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-06-01.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#ifndef GBuffer_hpp
#define GBuffer_hpp

#include <stdio.h>
#include <GL/glew.h>
#include "Shader.hpp"

class GBuffer
{
public:
    GBuffer();
    ~GBuffer();
    
    void Init(GLuint w, GLuint h, Shader* sh);
    void Render(glm::mat4* projection, glm::mat4* viewMatrix);
    void Reset();
    
    GLuint GetDiffuseTexture()  { return diffuseTexture; }
    GLuint GetPositionTexture() { return positionTexture; }
    GLuint GetNormalsTexture()  { return normalsTexture; }
    GLuint GetDepthTexture()    { return depthTexture; }
    
private:
    GLuint width, height;
    GLuint FBO;
    GLuint diffuseRT, positionRT, normalsRT, depthRT;
    GLuint diffuseTexture, positionTexture, normalsTexture, depthTexture;
    
    Shader* shader;
    GLuint uniformProjection, uniformView, uniformTexture;
};


#endif /* GBuffer_hpp */
