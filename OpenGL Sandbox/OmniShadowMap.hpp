//
//  OmniShadowMap.hpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-05-30.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#ifndef OmniShadowMap_hpp
#define OmniShadowMap_hpp

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
