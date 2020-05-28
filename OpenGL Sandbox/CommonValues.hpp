//
//  CommonValues.hpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-05-25.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#ifndef CommonValues_h
#define CommonValues_h

#include <cmath>
#include <algorithm>
#include <glm/glm.hpp>

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;
const float toRadians = 3.14159265f / 180.0f;

namespace CommonHelper
{

float uniform_randf(const float from, const float to);
    
float randf();
    
void concentricSampleDisk(float* dx, float* dy);
    
glm::vec3 cosineSampleHemisphere();

};

#endif /* CommonValues_h */
