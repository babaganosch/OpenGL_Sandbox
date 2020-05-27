//
//  OmniShadowMap.hpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-05-26.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#ifndef OmniShadowMap_hpp
#define OmniShadowMap_hpp

#include <stdio.h>
#include "ShadowMap.hpp"

class OmniShadowMap : public ShadowMap
{
public:
    OmniShadowMap();
    ~OmniShadowMap();
    
    bool Init(GLuint width, GLuint height);
    void Write();
    void Read(GLenum textureUnit);
};

#endif /* OmniShadowMap_hpp */
