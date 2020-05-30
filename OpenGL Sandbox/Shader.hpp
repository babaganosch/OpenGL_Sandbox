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
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonValues.hpp"

#include "DirectionalLight.hpp"
#include "PointLight.hpp"
#include "SpotLight.hpp"

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
    
    GLuint GetViewProjectionInverseMatrix() { return uniformViewProjectionInverseMatrix; }
    GLuint GetPreviousViewProjectionMatrix() { return uniformPreviousViewProjectionMatrix; }
    GLuint GetShowHalfScreenOnlyLocation() { return uniformShowHalfScreenOnly; }
    
    void SetDirectionalLight(DirectionalLight* dLight);
    void SetPointLights(PointLight* pLight, unsigned int lightCount);
    void SetSpotLights(SpotLight* sLight, unsigned int lightCount);
    void SetTexture(GLuint textureUnit);
    void SetDirectionalShadowMap(GLuint textureUnit);
    void SetDirectionalLightTransform(glm::mat4* lTransform);
    void SetMotionBlurTextures(GLuint textureUnit0, GLuint textureUnit1);
    
    void UseShader();
    void ClearShader();
    
private:
    int pointLightCount;
    int spotLightCount;
    
    GLuint shaderID, uniformProjection, uniformModel, uniformView, uniformEyePosition;
    GLuint uniformSpecularIntensity, uniformShininess;
    GLuint uniformTexture;
    GLuint uniformDirectionalLightTransform, uniformDirectionalShadowMap;
    
    GLuint uniformShowHalfScreenOnly;
    
    GLuint uniformViewProjectionInverseMatrix, uniformPreviousViewProjectionMatrix;
    GLuint uniformMotionBlurTexture0, uniformMotionBlurTexture1;
    
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
    
    GLuint uniformSpotLightCount;
    struct {
        GLuint uniformColour;
        GLuint uniformAmbientIntensity;
        GLuint uniformDiffuseIntensity;
        
        GLuint uniformPosition;
        GLuint uniformConstant;
        GLuint uniformLinear;
        GLuint uniformExponent;
        
        GLuint uniformDirection;
        GLuint uniformEdge;
    } uniformSpotLight[MAX_SPOT_LIGHTS];
    
    
    void CompileShader(const char* vertexCode, const char* fragmentCode);
    void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};

#endif /* Shader_hpp */
