//
//  Texture.hpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-05-24.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#ifndef Texture_hpp
#define Texture_hpp

#include <stdio.h>
#include <GL/glew.h>
#include "External/stb_image.h"

class Texture
{
public:
    Texture();
    Texture(const char* fileLoc);
    ~Texture();
    
    bool LoadTexture();
    bool LoadTextureA();
    void UseTexture();
    void ClearTexture();
    
private:
    GLuint textureID;
    int width, height, bitDepth;
    
    const char* fileLocation;
};

#endif /* Texture_hpp */
