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
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
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
    
    if ( glewInit( ) != GLEW_OK )
    {
        printf("Failed to initialize GLEW");
        glfwTerminate( );
        return -1;
    }
    
    glEnable(GL_DEPTH_TEST);
    
    glViewport( 0, 0, bufferWidth, bufferHeight );
    return 0;
}

Window::~Window()
{
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}
