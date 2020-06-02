//
//  CommonValues.cpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-05-28.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#include "CommonValues.hpp"

float CommonHelper::uniform_randf(const float from, const float to)
{
    return from + (to - from) * float(rand()) / float(RAND_MAX);
}

float CommonHelper::randf()
{
    return float(rand()) / float(RAND_MAX);
}

float CommonHelper::lerp(float a, float b, float t)
{
    return a + (t * (b - a));
}

void CommonHelper::concentricSampleDisk(float* dx, float* dy)
{
    float r, theta;
    float u1 = randf();
    float u2 = randf();
    // Map uniform random numbers to $[-1,1]^2$
    float sx = 2 * u1 - 1;
    float sy = 2 * u2 - 1;
    // Map square to $(r,\theta)$
    // Handle degeneracy at the origin
    if(sx == 0.0 && sy == 0.0)
    {
        *dx = 0.0;
        *dy = 0.0;
        return;
    }
    if(sx >= -sy)
    {
        if(sx > sy)
        { // Handle first region of disk
            r = sx;
            if(sy > 0.0)
                theta = sy / r;
            else
                theta = 8.0f + sy / r;
        }
        else
        { // Handle second region of disk
            r = sy;
            theta = 2.0f - sx / r;
        }
    }
    else
    {
        if(sx <= sy)
        { // Handle third region of disk
            r = -sx;
            theta = 4.0f - sy / r;
        }
        else
        { // Handle fourth region of disk
            r = -sy;
            theta = 6.0f + sx / r;
        }
    }
    theta *= float(M_PI) / 4.0f;
    *dx = r * cosf(theta);
    *dy = r * sinf(theta);
}

glm::vec3 CommonHelper::cosineSampleHemisphere()
{
    glm::vec3 ret;
    concentricSampleDisk(&ret.x, &ret.y);
    ret.z = sqrt(glm::max(0.0f, 1.0f - ret.x * ret.x - ret.y * ret.y));
    return ret;
}
