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
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "CommonValues.hpp"

#define hemisphereSamples 16

class Renderer
{
public:
    Renderer();
    ~Renderer();
    
    void Init(unsigned int width, unsigned int height);
    void RenderToTexture();
    void RenderSSAO(GLuint positionTexture, GLuint normalTexture);
    void RenderBlurredSSAO();
    void Reset();
    void Clear();
    
    GLuint GetTexture() { return colorTexture; }
    GLuint GetSSAOTexture() { return ssaoBlurredTexture; }
    GLuint GetNoiseTexture() { return noiseTexture; }
    glm::vec3* GetSSAOsamples() { return samples; }
    
private:
    GLuint FBO, RBO, ssaoBlurFBO;
    GLuint ssaoFBO;
    GLuint colorTexture, noiseTexture, ssaoTexture;
    GLuint ssaoBlurredTexture;
    int width;
    int height;
    
    glm::vec3 samples[hemisphereSamples];
};

#endif /* Renderer_hpp */
