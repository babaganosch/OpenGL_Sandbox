//
//  ParticleSystem.cpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-05-27.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#define _USE_MATH_DEFINES
#include "ParticleSystem.hpp"
#include "CommonValues.hpp"
#include <cmath>

using namespace glm;

ParticleSystem::~ParticleSystem()
{
    
}

void ParticleSystem::Kill(int id)
{
    particles[id] = particles.back();
    particles.pop_back();
}

void ParticleSystem::Spawn(Particle particle)
{
    particles.push_back(particle);
}

void ParticleSystem::Init(glm::vec3 offset)
{
    positionOffset = offset;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
        glBufferData(GL_ARRAY_BUFFER, 100000 * sizeof(vec4), nullptr, GL_STATIC_DRAW);
        glGenVertexArrays(1, &VAO);
    
        glBindVertexArray(VAO);
            glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, 0);
            glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleSystem::RenderParticles(glm::mat4 viewMatrix)
{
    unsigned int active_particles = (unsigned int)particles.size();
    std::vector<glm::vec4> stdData;
    
    glm::mat4 rotatedMatrix = glm::rotate(viewMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotatedMatrix = glm::translate(rotatedMatrix, positionOffset);
    
    for (int i = 0; i < active_particles; i++)
    {
        glm::vec3 transformedPositions = rotatedMatrix * vec4(particles[i].position, 1.0f);
        vec4 SUP = vec4(transformedPositions, particles[i].current_life / particles[i].maxlife);
        stdData.push_back(SUP);
    }
    
    std::sort(stdData.begin(), std::next(stdData.begin(), active_particles), [](const vec4& lhs, const vec4& rhs) { return lhs.z < rhs.z; });
    
    for (int i = 0; i < active_particles; i++)
    {
        data[i] = stdData.back();
        stdData.pop_back();
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, active_particles * sizeof(vec4), data);
        glBindVertexArray(VAO);
            glEnable(GL_PROGRAM_POINT_SIZE);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDepthMask(GL_FALSE);
            glDrawArrays(GL_POINTS, 0, active_particles);
            glDisable(GL_BLEND);
            glDepthMask(GL_TRUE);
        glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleSystem::ProcessParticles(float dt)
{
    for (int i = 0; i < particles.size(); i++)
    {
        
        particles[i].current_life += dt;
        if (particles[i].current_life >= particles[i].maxlife)
        {
            Kill(i);
        }
    }
    for (int i = 0; i < particles.size(); i++)
    {
        particles[i].position += dt * particles[i].velocity;
    }
    
    unsigned int missing = max_size - (unsigned int)particles.size();
    missing = std::fmin(missing, 128);
    if (missing > 0)
    {
        vec3 startpos = vec3(0.0f, 0.0f, 0.0f);
        float scale = 3.0f;
        
        for (int i = 0; i < missing; i++)
        {
            const float theta = CommonHelper::uniform_randf(0.0f, 2.0f * M_PI);
            const float u = CommonHelper::uniform_randf(0.975f, 1.0f);
            
            vec3 velocity = vec3(u, sqrt(1.0f - u * u) * cosf(theta), sqrt(1.0f - u * u) * sinf(theta));
            float maxlife = CommonHelper::uniform_randf(1.5f, 2.0f);
            float currentLife = CommonHelper::uniform_randf(0.0f, maxlife-0.2);
            velocity *= scale;
            
            Particle p = Particle(currentLife, maxlife, velocity, startpos);
            Spawn(p);
        }
    }
}
