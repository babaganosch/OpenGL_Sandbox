//
//  Shader.cpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-05-20.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#include "Shader.hpp"

Shader::Shader()
{
    shaderID = 0;
    uniformModel = 0;
    uniformProjection = 0;
    uniformDirectionalLight.uniformColour = 0;
    uniformDirectionalLight.uniformAmbientIntensity = 0;
    uniformDirectionalLight.uniformDiffuseIntensity = 0;
    uniformDirectionalLight.uniformDirection = 0;
    uniformSpecularIntensity = 0;
    uniformShininess = 0;
    uniformEyePosition = 0;
    
    pointLightCount = 0;
}

Shader::~Shader()
{
    ClearShader();
}

GLuint Shader::GetProjectionLocation()
{
    return uniformProjection;
}

GLuint Shader::GetModelLocation()
{
    return uniformModel;
}

GLuint Shader::GetViewLocation()
{
    return uniformView;
}

GLuint Shader::GetAmbientIntensityLocation()
{
    return uniformDirectionalLight.uniformAmbientIntensity;
}

GLuint Shader::GetAmbientColourLocation()
{
    return uniformDirectionalLight.uniformColour;
}

GLuint Shader::GetDiffuseIntensityLocation()
{
    return uniformDirectionalLight.uniformDiffuseIntensity;
}

GLuint Shader::GetDirectionLocation()
{
    return uniformDirectionalLight.uniformDirection;
}

GLuint Shader::GetSpecularIntensityLocation()
{
    return uniformSpecularIntensity;
}

GLuint Shader::GetShininessLocation()
{
    return uniformShininess;
}

GLuint Shader::GetEyePositionLocation()
{
    return uniformEyePosition;
}

void Shader::SetDirectionalLight(DirectionalLight* dLight)
{
    dLight->UseLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColour, uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}

void Shader::SetPointLights(PointLight* pLight, unsigned int lightCount)
{
    if (lightCount > MAX_POINT_LIGHTS) lightCount = MAX_POINT_LIGHTS;
    
    glUniform1i(uniformPointLightCount, lightCount);
    
    for (unsigned int i = 0; i < lightCount; i++)
    {
        pLight[i].UseLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColour, uniformPointLight[i].uniformDiffuseIntensity, uniformPointLight[i].uniformPosition, uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent);
    }
}

void Shader::UseShader()
{
    glUseProgram(shaderID);
}

void Shader::ClearShader()
{
    if (shaderID != 0)
    {
        glDeleteProgram(shaderID);
        shaderID = 0;
    }
    
    uniformModel = 0;
    uniformProjection = 0;
    uniformDirectionalLight.uniformColour = 0;
    uniformDirectionalLight.uniformAmbientIntensity = 0;
    uniformDirectionalLight.uniformDiffuseIntensity = 0;
    uniformDirectionalLight.uniformDirection = 0;
    uniformSpecularIntensity = 0;
    uniformShininess = 0;
    uniformEyePosition = 0;
    
    pointLightCount = 0;
}

std::string Shader::ReadFile(const char* fileLocation)
{
    std::string content;
    std::ifstream fileStream(fileLocation, std::ios::in);
    
    if (!fileStream)
    {
        printf("Failed to read '%s'! File doesn't exist.", fileLocation);
        return "";
    }
    
    std::string line = "";
    while (!fileStream.eof())
    {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }
    
    fileStream.close();
    return content;
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
    std::string vertexString = ReadFile(vertexLocation);
    std::string fragmentString = ReadFile(fragmentLocation);
    const char* vertexCode = vertexString.c_str();
    const char* fragmentCode = fragmentString.c_str();
    CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
    CompileShader(vertexCode, fragmentCode);
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
    shaderID = glCreateProgram();
    
    if (!shaderID) {
        printf("Error creating shader program!\n");
        return;
    }
    
    AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
    AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);
    
    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    
    glLinkProgram(shaderID);
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), nullptr, eLog);
        printf("Error linking program: '%s'\n", eLog);
        return;
    }
    
    glValidateProgram(shaderID);
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), nullptr, eLog);
        printf("Error validating program: '%s'\n", eLog);
        return;
    }
    
    uniformModel = glGetUniformLocation(shaderID, "model");
    uniformProjection = glGetUniformLocation(shaderID, "projection");
    uniformView = glGetUniformLocation(shaderID, "view");
    uniformDirectionalLight.uniformColour = glGetUniformLocation(shaderID, "directionalLight.base.colour");
    uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
    uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
    uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
    uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
    uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
    uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");
    
    uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");
    
    for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        char locBuff[100] = { '\0' };
        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.colour", (int)i);
        uniformPointLight[i].uniformColour = glGetUniformLocation(shaderID, locBuff);
        
        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", (int)i);
        uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);
        
        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", (int)i);
        uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);
        
        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", (int)i);
        uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);
        
        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", (int)i);
        uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);
        
        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", (int)i);
        uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);
        
        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", (int)i);
        uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);
    }
}

void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);
    
    const GLchar* theCode[1];
    theCode[0] = shaderCode;
    
    GLint codeLength[1];
    codeLength[0] = (GLint) strlen(shaderCode);
    
    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);
    
    GLint result = 0;
    GLchar eLog[1024] = { 0 };
    
    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(theShader, sizeof(eLog), nullptr, eLog);
        printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
        return;
    }
    
    glAttachShader(theProgram, theShader);
}
