//
//  Window.hpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-05-20.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#ifndef Window_hpp
#define Window_hpp

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
    Window();
    Window(GLint windowWidth, GLint windowHeight);
    ~Window();
    
    int Initialise();
    
    GLfloat getBufferWidth()  { return bufferWidth; }
    GLfloat getBufferHeight() { return bufferHeight; }
    
    GLfloat getXChange();
    GLfloat getYChange();
    bool* getKeys() { return keys; }
    bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }
    void swapBuffers() { glfwSwapBuffers(mainWindow); };
    
private:
    GLFWwindow* mainWindow;
    GLint width, height;
    GLint bufferWidth, bufferHeight;
    
    GLfloat lastX, lastY, xChange, yChange;
    bool mouseFirstMoved;
    
    bool keys[1024];
    
    void createCallbacks();
    static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
    static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};

#endif /* Window_hpp */
