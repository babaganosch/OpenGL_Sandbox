//
//  Shader.hpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-05-20.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

#include "CommonValues.hpp"

#include "DirectionalLight.hpp"
#include "PointLight.hpp"

class Shader {
public:
    Shader();
    ~Shader();
    
    void CreateFromString(const char* vertexCode, const char* fragmentCode);
    void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);
    std::string ReadFile(const char* fileLocation);
    GLuint GetProjectionLocation();
    GLuint GetModelLocation();
    GLuint GetViewLocation();
    GLuint GetAmbientIntensityLocation();
    GLuint GetAmbientColourLocation();
    GLuint GetDiffuseIntensityLocation();
    GLuint GetDirectionLocation();
    GLuint GetSpecularIntensityLocation();
    GLuint GetShininessLocation();
    GLuint GetEyePositionLocation();
    
    void SetDirectionalLight(DirectionalLight* dLight);
    void SetPointLights(PointLight* pLight, unsigned int lightCount);
    
    void UseShader();
    void ClearShader();
    
private:
    int pointLightCount;
    
    GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformEyePosition;
    GLuint uniformSpecularIntensity, uniformShininess;
    
    struct {
        GLuint uniformColour;
        GLuint uniformAmbientIntensity;
        GLuint uniformDiffuseIntensity;
        
        GLuint uniformDirection;
    } uniformDirectionalLight;
    
    GLuint uniformPointLightCount;
    struct {
        GLuint uniformColour;
        GLuint uniformAmbientIntensity;
        GLuint uniformDiffuseIntensity;
        
        GLuint uniformPosition;
        GLuint uniformConstant;
        GLuint uniformLinear;
        GLuint uniformExponent;
    } uniformPointLight[MAX_POINT_LIGHTS];
    
    void CompileShader(const char* vertexCode, const char* fragmentCode);
    void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};

#endif /* Shader_hpp */
