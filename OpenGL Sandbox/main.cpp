#define STB_IMAGE_IMPLEMENTATION

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

#include "CommonValues.hpp"

#include "Mesh.hpp"
#include "Shader.hpp"
#include "Window.hpp"
#include "Camera.hpp"
#include "Texture.hpp"
#include "DirectionalLight.hpp"
#include "PointLight.hpp"
#include "SpotLight.hpp"
#include "Material.hpp"

#include "ParticleSystem.hpp"
#include "Quad.hpp"

#include "Model.hpp"

using namespace glm;
using namespace std;

GLuint uniformModel = 0, uniformProjection = 0, uniformView = 0, uniformEyePosition = 0;
GLuint uniformSpecularIntensity = 0, uniformShininess = 0;

Window mainWindow;
Camera camera;
vector<Mesh*> meshList;
vector<Shader> shaderList;
Shader directionalShadowShader;
Shader particleShader;
Shader passthrough;

ParticleSystem ps;
Quad screenQuad;

Texture brickTexture;
Texture dirtTexture;
Texture grassTexture;
Texture plainTexture;
Texture particleTexture;

Material shinyMaterial;
Material dullMaterial;

Model xwing;
Model blackHawk;
Model deLorean;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

GLfloat blackHawkAngle = 0.0f;

// Vertex Shader
static const char* vShader = "./Shaders/shader.vert";
// Fragment Shader
static const char* fShader = "./Shaders/shader.frag";

void CalcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset)
{
    for (size_t i = 0; i < indiceCount; i += 3)
    {
        unsigned int in0 = indices[i] * vLength;
        unsigned int in1 = indices[i+1] * vLength;
        unsigned int in2 = indices[i+2] * vLength;
        vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
        vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
        vec3 normal = cross(v1, v2);
        normal = normalize(normal);
        
        in0 += normalOffset;
        in1 += normalOffset;
        in2 += normalOffset;
        
        vertices[in0] += normal.x; vertices[in0+1] += normal.y; vertices[in0+2] += normal.z;
        vertices[in1] += normal.x; vertices[in1+1] += normal.y; vertices[in1+2] += normal.z;
        vertices[in2] += normal.x; vertices[in2+1] += normal.y; vertices[in2+2] += normal.z;
    }
    
    for (size_t i = 0; i < verticeCount/vLength; i++)
    {
        unsigned int nOffset = (unsigned int)i * vLength + normalOffset;
        vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
        vec = normalize(vec);
        vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
    }
}

void CreateObjects()
{
    
    unsigned int indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };
    
    GLfloat vertices[] = {
        // x    y      z        u     v         normals xyz
        -1.0f, -1.0f, -0.6f,    0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
         0.0f, -1.0f,  1.0f,    0.5f, 0.0f,     0.0f, 0.0f, 0.0f,
         1.0f, -1.0f, -0.6f,    1.0f, 0.0f,     0.0f, 0.0f, 0.0f,
         0.0f,  1.0f,  0.0f,    0.5f, 1.0f,     0.0f, 0.0f, 0.0f
    };
    
    unsigned int floorIndices[] = {
        0, 2, 1,
        1, 2, 3
    };
    
    GLfloat floorVertices[] = {
        -100.0f,  0.0f, -100.0f,  0.0f,  0.0f,    0.0f, -1.0f, 0.0f,
         100.0f,  0.0f, -100.0f,  100.0f, 0.0f,   0.0f, -1.0f, 0.0f,
        -100.0f,  0.0f,  100.0f,  0.0f,  100.0f,  0.0f, -1.0f, 0.0f,
         100.0f,  0.0f,  100.0f,  100.0f, 100.0f, 0.0f, -1.0f, 0.0f
    };
    
    CalcAverageNormals(indices, 12, vertices, 32, 8, 5);
    
    Mesh* obj1 = new Mesh();
    obj1->CreateMesh(vertices, indices, 32, 12);
    meshList.push_back(obj1);
    
    Mesh* obj2 = new Mesh();
    obj2->CreateMesh(vertices, indices, 32, 12);
    meshList.push_back(obj2);
    
    Mesh* obj3 = new Mesh();
    obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
    meshList.push_back(obj3);
}

void CreateShaders()
{
    Shader* shader1 = new Shader();
    shader1->CreateFromFiles(vShader, fShader);
    shaderList.push_back(*shader1);
    
    directionalShadowShader = Shader();
    directionalShadowShader.CreateFromFiles("Shaders/directional_shadow_map.vert", "Shaders/directional_shadow_map.frag");
    
    particleShader = Shader();
    particleShader.CreateFromFiles("Shaders/particle.vert", "Shaders/particle.frag");
    
    passthrough = Shader();
    passthrough.CreateFromFiles("Shaders/QuadRenderer/passthrough.vert", "Shaders/QuadRenderer/simpleTexture.frag");
}

void RenderScene()
{
    /* TRI1 */
    mat4 model(1.0f);
    model = translate(model, vec3(0.0f, 0.0f, -2.5f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model));
    brickTexture.UseTexture();
    shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[0]->RenderMesh();

    /* TRI2 */
    model = mat4(1.0f);
    model = translate(model, vec3(0.0f, 4.0f, -2.5f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model));
    dirtTexture.UseTexture();
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[1]->RenderMesh();

    /* FLOOR */
    model = mat4(1.0f);
    model = translate(model, vec3(0.0f, -2.0f, 0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model));
    dirtTexture.UseTexture();
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[2]->RenderMesh();

    /* XWING */
    model = mat4(1.0f);
    model = translate(model, vec3(-7.0f, 0.0f, 10.0f));
    model = scale(model, vec3(0.006f, 0.006f, 0.006f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model));
    shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    xwing.RenderModel();

    /* BLACKHAWK */
    blackHawkAngle += 0.1f;
    if (blackHawkAngle > 360.0f) blackHawkAngle = 0.1f;
    model = mat4(1.0f);
    model = rotate(model, -blackHawkAngle * toRadians, vec3(0.0f, 1.0f, 0.0f));
    model = translate(model, vec3(-7.0f, 2.0f, 0.0f));
    model = scale(model, vec3(0.4f, 0.4f, 0.4f));
    model = rotate(model, -15.0f * toRadians, vec3(0.0f, 0.0f, 1.0f));
    model = rotate(model, -90.0f * toRadians, vec3(1.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model));
    shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    blackHawk.RenderModel();
    
    /* DELOREAN */
    model = mat4(1.0f);
    model = translate(model, vec3(3.0f, -1.5f, 4.0f));
    model = scale(model, vec3(0.005f, 0.005f, 0.005f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model));
    shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    deLorean.RenderModel();
    
}

void DirectionalShadowMapPass(DirectionalLight* light)
{
    directionalShadowShader.UseShader();
    
    glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());
    
    light->GetShadowMap()->Write();
    glClear(GL_DEPTH_BUFFER_BIT);
    
    uniformModel = directionalShadowShader.GetModelLocation();
    mat4 matrix = light->CalculateLightTransform();
    directionalShadowShader.SetDirectionalLightTransform(&matrix);
    
    RenderScene();
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(mat4 projectionMatrix, mat4 viewMatrix)
{
    
    shaderList[0].UseShader();
    
    uniformModel = shaderList[0].GetModelLocation();
    uniformProjection = shaderList[0].GetProjectionLocation();
    uniformView = shaderList[0].GetViewLocation();
    uniformEyePosition = shaderList[0].GetEyePositionLocation();
    uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
    uniformShininess = shaderList[0].GetShininessLocation();
    
    mainWindow.resetViewport();
    
    screenQuad.RenderToTexture();
    
    // Clear the window
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glEnable(GL_DEPTH_TEST);
    
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, value_ptr(projectionMatrix));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, value_ptr(viewMatrix));
    glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

    shaderList[0].SetDirectionalLight(&mainLight);
    shaderList[0].SetPointLights(pointLights, pointLightCount);
    shaderList[0].SetSpotLights(spotLights, spotLightCount);
    mat4 matrix = mainLight.CalculateLightTransform();
    shaderList[0].SetDirectionalLightTransform(&matrix);
    
    mainLight.GetShadowMap()->Read(GL_TEXTURE1);
    shaderList[0].SetTexture(0);
    shaderList[0].SetDirectionalShadowMap(1);
    
    // Let one spotlight follow the camera
    glm::vec3 handPosition = camera.getCameraPosition();
    handPosition.y -= 0.3f;
    spotLights[0].SetFlash(handPosition, camera.getCameraDirection());
    
    RenderScene();
}

int main() {
    
    mainWindow = Window(1366, 768); // 1280x1024 also works good
    mainWindow.Initialise();
    
    CreateObjects();
    glBindVertexArray(meshList[0]->GetVAO());   // Fix for validation fail
    CreateShaders();
    glBindVertexArray(0);
    
    ps = ParticleSystem(1000);
    ps.Init();
    
    screenQuad = Quad();
    screenQuad.Init(mainWindow.getBufferWidth(), mainWindow.getBufferHeight());
    
    camera = Camera(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), -88.0f, 0.0f, 5.0f, 0.5f);
    
    brickTexture = Texture((char*)"Textures/brick.png");
    brickTexture.LoadTextureA();
    dirtTexture = Texture((char*)"Textures/dirt.png");
    dirtTexture.LoadTextureA();
    plainTexture = Texture((char*)"Textures/plain.png");
    plainTexture.LoadTextureA();
    grassTexture = Texture((char*)"Textures/grass.jpg");
    grassTexture.LoadTexture();
    particleTexture = Texture((char*)"Textures/plain.png");
    particleTexture.LoadTextureA();
    
    shinyMaterial = Material(4.0f, 156.0f);
    dullMaterial = Material(0.3f, 4.0f);
    
    xwing = Model();
    xwing.LoadModel("Models/x-wing.obj");
    
    blackHawk = Model();
    blackHawk.LoadModel("Models/uh60.obj");
    
    deLorean = Model();
    deLorean.LoadModel("Models/DeLorean.obj");
    
    mainLight = DirectionalLight(2048, 2048,
                                 1.0f, 1.0f, 1.0f,
                                 0.2f, 0.3f,
                                 0.0f, -15.0f, -10.0f);
    
    /*
    pointLights[0] = PointLight(0.0f, 0.0f, 1.0f,
                                0.0f, 0.1f,
                                0.0f, 0.0f, 0.0f,
                                0.3f, 0.2f, 0.1f);
    pointLightCount++;
    pointLights[1] = PointLight(0.0f, 1.0f, 0.0f,
                                0.0f, 0.1f,
                                -4.0f, 2.0f, 0.0f,
                                0.3f, 0.1f, 0.1f);
    pointLightCount++;
    
    
    spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
                              0.0f, 2.0f,
                              0.0f, 0.0f, 0.0f,
                              0.0f, -1.0f, 0.0f,
                              1.0f, 0.0f, 0.0f,
                              28.0f);
    spotLightCount++;
    
    spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f,
                              0.0f, 1.0f,
                              0.0f, -1.5f, 0.0f,
                              -100.0f, -1.0f, 0.0f,
                              1.0f, 0.0f, 0.0f,
                              20.0f);
    spotLightCount++;
    */
    mat4 projection = perspective(45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.f);
    
    while ( !mainWindow.getShouldClose() )
    {
        // Timing
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;
        
        // Input
        glfwPollEvents( );
        camera.keyControl(deltaTime, mainWindow.getKeys());
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
        
        DirectionalShadowMapPass(&mainLight);
        RenderPass(projection, camera.calculateViewMatrix());
        
        
        ps.ProcessParticles(deltaTime);
        particleShader.UseShader();
        
        plainTexture.UseTexture();
        glUniformMatrix4fv(particleShader.GetProjectionLocation(), 1, GL_FALSE, value_ptr(projection));
        ps.RenderParticles(camera.calculateViewMatrix());
        
         
        passthrough.UseShader();
        screenQuad.RenderTextureToScreen();
        
        glUseProgram(0);
        
        // Swap buffers
        mainWindow.swapBuffers();
    }
    
    glfwTerminate( );
    
    return 0;
}
