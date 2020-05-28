//
//  ParticleSystem.hpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-05-27.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#ifndef ParticleSystem_hpp
#define ParticleSystem_hpp

#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Particle.hpp"

class ParticleSystem
{
public:
    std::vector<Particle> particles;
    int max_size;
    
    ParticleSystem() : max_size(0) { }
    explicit ParticleSystem(int size) : max_size(size) { }
    ~ParticleSystem();
    
    void Init();
    
    void Kill(int id);
    void Spawn(Particle particle);
    void ProcessParticles(float dt);
    void RenderParticles(glm::mat4 viewMatrix);
    
private:
    GLuint VAO, VBO;
    glm::vec4 data[10000] = {};
};

#endif /* ParticleSystem_hpp */
