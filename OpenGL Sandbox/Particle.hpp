//
//  Particle.hpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-05-27.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#ifndef Particle_hpp
#define Particle_hpp

#include <stdio.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Particle
{
public:
    GLfloat maxlife;
    GLfloat current_life;
    
    glm::vec3 velocity;
    glm::vec3 position;
    
    Particle();
    Particle(float currentlife, float maxlife, glm::vec3 velocity, glm::vec3 position);
    ~Particle();
    
private:

    
};

#endif /* Particle_hpp */
