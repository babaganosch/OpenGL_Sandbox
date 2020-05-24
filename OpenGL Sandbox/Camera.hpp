//
//  Camera.hpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-05-23.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#ifndef Camera_hpp
#define Camera_hpp

#include <stdio.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera
{
public:
    Camera();
    Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);
    ~Camera();
    
    void keyControl(GLfloat dt, bool* keys);
    void mouseControl(GLfloat xChange, GLfloat yChange);
    glm::mat4 calculateViewMatrix();
    
private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    
    GLfloat yaw;
    GLfloat pitch;
    
    GLfloat moveSpeed;
    GLfloat turnSpeed;
    
    void update();
};

#endif /* Camera_hpp */