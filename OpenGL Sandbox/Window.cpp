//
//  Window.cpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-05-20.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#include "Window.hpp"

Window::Window()
{
    width  = 800;
    height = 600;
    xChange = 0.0f;
    yChange = 0.0f;
    lastX = 0.0f;
    lastY = 0.0f;
    mouseFirstMoved = true;
    
    for (size_t i = 0; i < 1024; i++)
    {
        keys[i] = 0;
    }
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
    Window();
    width  = windowWidth;
    height = windowHeight;
}

int Window::Initialise()
{
    glfwInit( );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    mainWindow = glfwCreateWindow( width, height, "OpenGL Sandbox!", nullptr, nullptr);
    
    if ( mainWindow == nullptr )
    {
        printf("Failed to create GLFW window!");
        glfwTerminate( );
        return -1;
    }
    
    glfwGetFramebufferSize( mainWindow, &bufferWidth, &bufferHeight);
    
    glfwMakeContextCurrent( mainWindow );
    glewExperimental = GL_TRUE;
    
    // Handle key + mouse inputs
    createCallbacks();
    glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    if ( glewInit( ) != GLEW_OK )
    {
        printf("Failed to initialize GLEW");
        glfwTerminate( );
        return -1;
    }
    
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    
    glViewport( 0, 0, bufferWidth, bufferHeight );
    
    glfwSetWindowUserPointer(mainWindow, this);
    
    return 0;
}

void Window::resetViewport()
{
    glViewport( 0, 0, bufferWidth, bufferHeight );
}

GLfloat Window::getXChange()
{
    GLfloat theChange = xChange;
    xChange = 0.0f;
    return theChange;
}

GLfloat Window::getYChange()
{
    GLfloat theChange = yChange;
    yChange = 0.0f;
    return theChange;
}

void Window::createCallbacks()
{
    glfwSetKeyCallback(mainWindow, handleKeys);
    glfwSetCursorPosCallback(mainWindow, handleMouse);
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
    
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            theWindow->keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            theWindow->keys[key] = false;
        }
    }
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
    Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
    
    if (theWindow->mouseFirstMoved)
    {
        theWindow->lastX = xPos;
        theWindow->lastY = yPos;
        
        theWindow->mouseFirstMoved = false;
    }
    
    theWindow->xChange = xPos - theWindow->lastX;
    theWindow->yChange = theWindow->lastY - yPos; // This way to not have inverted controlls
    theWindow->lastX = xPos;
    theWindow->lastY = yPos;
}

Window::~Window()
{
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}
