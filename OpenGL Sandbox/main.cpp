#include <iostream>
#include <string.h>
#include <cmath>
#include <vector>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.hpp"
#include "Shader.hpp"
#include "Window.hpp"

using namespace glm;
using namespace std;

const float toRadians = 3.14159265f / 180.0f;
Window mainWindow;
vector<Mesh*> meshList;
vector<Shader> shaderList;

// Vertex Shader	
static const char* vShader = "./Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "./Shaders/shader.frag";

void CreateObjects()
{
    
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };
    
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
         0.0f, -1.0f, 1.0f,
         1.0f, -1.0f, 0.0f,
         0.0f,  1.0f, 0.0f
    };
    
    Mesh* obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(obj1);
    
    Mesh* obj2 = new Mesh();
    obj2->CreateMesh(vertices, indices, 12, 12);
    meshList.push_back(obj1);
}

void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
}

int main() {
    
    mainWindow = Window(800, 600);
    mainWindow.Initialise();
    
    CreateObjects();
    glBindVertexArray(meshList[0]->GetVAO());   // Fix for validation fail
    CreateShaders();
    glBindVertexArray(0);
    
    GLuint uniformModel = 0, uniformProjection = 0;
    mat4 projection = perspective(45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.f);
    
    while ( !mainWindow.getShouldClose() )
    {
        glfwPollEvents( );
        
        glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        
        shaderList[0].UseShader();
        uniformModel = shaderList[0].GetModelLocation();
        uniformProjection = shaderList[0].GetProjectionLocation();
        
            mat4 model(1.0f);
            model = translate(model, vec3(0.0f, 0.0f, -2.5f));
            model = scale(model, vec3(0.4f, 0.4f, 0.4f));
        
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model));
            glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, value_ptr(projection));
        
            meshList[0]->RenderMesh();
        
            model = mat4(1.0f);
            model = translate(model, vec3(0.0f, 1.0f, -2.5f));
            model = scale(model, vec3(0.4f, 0.4f, 0.4f));
            glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model));
        
            meshList[1]->RenderMesh();
        
        glUseProgram(0);
        
        mainWindow.swapBuffers();
    }
    
    glfwTerminate( );
    
    return 0;
}
