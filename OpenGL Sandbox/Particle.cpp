//
//  Particle.cpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-05-27.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#include "Particle.hpp"

Particle::Particle(float currentlife, float maxlife, glm::vec3 velocity, glm::vec3 position)
{
    this->maxlife = maxlife;
    this->current_life = currentlife;
    this->velocity = velocity;
    this->position = position;
}

Particle::~Particle()
{
    
}
