//
//  GameHandler.hpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-06-01.
//  Copyright � 2020 Lars Andersson. All rights reserved.
//

#ifndef GameHandler_hpp
#define GameHandler_hpp

#include <stdio.h>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "Window.hpp"
#include "PointLight.hpp"
#include "Player.hpp"

class GameHandler
{
public:
    GameHandler();
    ~GameHandler();

    void Update(Window* window, PointLight* light, GLfloat dt);
    glm::mat4 GetModelMatrix();
    Player* GetPlayer() { return &player; }

private:
    Player player;

    GLfloat score, maxScore;
};

#endif /* GameHandler.hpp */

