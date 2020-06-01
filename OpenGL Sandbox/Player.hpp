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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Window.hpp"

class Player
{
public:
    Player();
    ~Player();
    
    void Update(Window* window, GLfloat dt);
    glm::mat4 GetModelMatrix();
    
private:
    glm::vec3 position, scale;
    GLfloat angle;
};

#endif /* Player_hpp */
