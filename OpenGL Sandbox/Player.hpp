//
//  Player.hpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-06-01.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "Window.hpp"

class Player
{
public:
    Player();
    ~Player();
    
    void Update(Window* window, GLfloat dt);
    glm::mat4 GetModelMatrix();
    glm::vec3 GetModelPosition();
    
private:
    glm::mat4 model;
    glm::vec3 position, scale;
    
    GLfloat angle, maxSpeed, rotateSpeed, acceleration, currentSpeed;
};

#endif /* Player_hpp */