#define STB_IMAGE_IMPLEMENTATION

#if DEBUG
#define checkGLError() CheckErrors()
#else
#define checkGLError()
#endif

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

#include "Renderer.hpp"

#include "ParticleSystem.hpp"
#include "Quad.hpp"

#include "Model.hpp"

using namespace glm;
using namespace std;

GLuint uniformModel = 0, uniformProjection = 0, uniformView = 0, uniformEyePosition = 0;
GLuint uniformSpecularIntensity = 0, uniformShininess = 0;
GLuint uniformOmniLightPos = 0, uniformFarPlane = 0;

Window mainWindow;
Camera camera;
vector<Mesh*> meshList;
vector<Shader> shaderList;
Shader directionalShadowShader;
Shader omniShadowShader;
Shader particleShader;
Shader passthrough;
Shader motionBlurShader;
Shader chrAbrShader;

Renderer renderer;

ParticleSystem ps0;
ParticleSystem ps1;
ParticleSystem ps2;
ParticleSystem ps3;
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
Model ball;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
GLfloat blackHawkAngle = 0.0f;

bool activeOmniShadowPass = true;
bool activeMotionBlur = false;
bool activeParticles  = true;
bool showHalfScreenOnly = false;

vec3 blueLightPos  = vec3(0.0f);
vec3 greenLightPos = vec3(-4.0f, 2.0f, 3.0f);

#if DEBUG
void CheckErrors()
{
    GLenum errCode;
    const GLubyte* errString;
    while ((errCode = glGetError()) != GL_NO_ERROR)
    {
        errString = gluErrorString(errCode);
        fprintf(stderr, "OpenGL Error: %s\n", errString);
    }
}
#endif

void CalcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount, unsigned int vLength, unsigned int normalOffset)
{
    for (size_t i = 0; i < indiceCount; i += 3)
    {
        unsigned int in0 = indices[i] * vLength;
        unsigned int in1 = indices[i+2] * vLength;
        unsigned int in2 = indices[i+1] * vLength;
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
        0, 1, 3,
        1, 2, 3,
        2, 0, 3,
        0, 2, 1
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
    shader1->CreateFromFiles("Shaders/shader.vert", "Shaders/shader.frag");
    shaderList.push_back(*shader1);
    
    directionalShadowShader = Shader();
    directionalShadowShader.CreateFromFiles("Shaders/directional_shadow_map.vert", "Shaders/directional_shadow_map.frag");
    
    omniShadowShader = Shader();
    omniShadowShader.CreateFromFiles("Shaders/OmniShadowMaps/omni_directional_shadow_map.vert", "Shaders/OmniShadowMaps/omni_directional_shadow_map.geom", "Shaders/OmniShadowMaps/omni_directional_shadow_map.frag");
    
    particleShader = Shader();
    particleShader.CreateFromFiles("Shaders/particle.vert", "Shaders/particle.frag");
    
    passthrough = Shader();
    passthrough.CreateFromFiles("Shaders/QuadRenderer/passthrough.vert", "Shaders/QuadRenderer/simpleTexture.frag");
    
    motionBlurShader = Shader();
    motionBlurShader.CreateFromFiles("Shaders/MotionBlur/motionBlur.vert", "Shaders/MotionBlur/motionBlur.frag");
    
    chrAbrShader = Shader();
    chrAbrShader.CreateFromFiles("Shaders/CHRABR/passthrough.vert", "Shaders/CHRABR/chromatic_aberration.frag");
}

void RenderScene()
{
    // TRI1
    mat4 model(1.0f);
    model = translate(model, vec3(0.0f, 0.0f, -2.5f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model));
    brickTexture.UseTexture();
    shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[0]->RenderMesh();
    
    // TRI2
    model = mat4(1.0f);
    model = translate(model, vec3(0.0f, 4.0f, -2.5f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model));
    dirtTexture.UseTexture();
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[1]->RenderMesh();

    // FLOOR
    model = mat4(1.0f);
    model = translate(model, vec3(0.0f, -2.0f, 0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model));
    dirtTexture.UseTexture();
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[2]->RenderMesh();

    // XWING
    model = mat4(1.0f);
    model = translate(model, vec3(-7.0f, 0.0f, 10.0f));
    model = scale(model, vec3(0.006f, 0.006f, 0.006f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model));
    shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    xwing.RenderModel();

    // BLACKHAWK
    model = mat4(1.0f);
    model = rotate(model, -blackHawkAngle * toRadians, vec3(0.0f, 1.0f, 0.0f));
    model = translate(model, vec3(-7.0f, 2.0f, 0.0f));
    model = scale(model, vec3(0.4f, 0.4f, 0.4f));
    model = rotate(model, -15.0f * toRadians, vec3(0.0f, 0.0f, 1.0f));
    model = rotate(model, -90.0f * toRadians, vec3(1.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model));
    shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    blackHawk.RenderModel();
    
    // DELOREAN
    model = mat4(1.0f);
    model = translate(model, vec3(3.0f, -1.5f, 4.0f));
    model = scale(model, vec3(0.005f, 0.005f, 0.005f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model));
    shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    deLorean.RenderModel();
    
    // BALL1
    model = mat4(1.0f);
    model = translate(model, vec3(-3.0f, -1.7f, 0.0f));
    model = scale(model, vec3(0.005f, 0.005f, 0.005f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model));
    shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    ball.RenderModel();
    
    // BALL2
    model = mat4(1.0f);
    model = translate(model, vec3(-3.0f, -1.7f, 1.25f));
    model = scale(model, vec3(0.005f, 0.005f, 0.005f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model));
    dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    ball.RenderModel();
    
    // LIGHTBALL 1 (BLUE)
    model = mat4(1.0f);
    model = translate(model, blueLightPos);
    model = scale(model, vec3(0.002f, 0.002f, 0.002f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model));
    shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    ball.RenderModel();
    
    // LIGHTBALL 2 (GREEN)
    model = mat4(1.0f);
    model = translate(model, greenLightPos);
    model = scale(model, vec3(0.002f, 0.002f, 0.002f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model));
    shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    ball.RenderModel();
    
}

void DirectionalShadowMapPass(DirectionalLight* light)
{
    directionalShadowShader.UseShader();
    
    glViewport(0, 0, light->GetShadowMap()->GetShadowWidth(), light->GetShadowMap()->GetShadowHeight());
    
    light->GetShadowMap()->Write();
    
    glClear(GL_DEPTH_BUFFER_BIT);
    
    uniformSpecularIntensity = directionalShadowShader.GetSpecularIntensityLocation();
    uniformShininess = directionalShadowShader.GetShininessLocation();
    uniformModel = directionalShadowShader.GetModelLocation();
    mat4 matrix = light->CalculateLightTransform();
    directionalShadowShader.SetDirectionalLightTransform(&matrix);
    
    RenderScene();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OmniShadowMapPass(PointLight* light)
{
    omniShadowShader.UseShader();
    
    light->GetShadowMap()->Write();
    glClear(GL_DEPTH_BUFFER_BIT);
    
    uniformSpecularIntensity = directionalShadowShader.GetSpecularIntensityLocation();
    uniformShininess = directionalShadowShader.GetShininessLocation();
    uniformModel = omniShadowShader.GetModelLocation();
    uniformOmniLightPos = omniShadowShader.GetOmniLightPosLocation();
    uniformFarPlane = omniShadowShader.GetFarPlaneLocation();
    
    vec3 lightPosition = light->GetPosition();
    glUniform3f(uniformOmniLightPos, lightPosition.x, lightPosition.y, lightPosition.z);
    glUniform1f(uniformFarPlane, light->GetFarPlane());
    omniShadowShader.SetLightMatrices(light->CalculateLightTransform());
    
    RenderScene();
    
    //glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ParticleRenderingPass(mat4 projectionMatrix, mat4 viewMatrix)
{
    ps0.ProcessParticles(deltaTime);
    ps1.ProcessParticles(deltaTime);
    ps2.ProcessParticles(deltaTime);
    ps3.ProcessParticles(deltaTime);
    particleShader.UseShader();
    particleTexture.UseTexture();
    glUniform1i(particleShader.GetTheTextureLocation(), 1);
    glUniformMatrix4fv(particleShader.GetProjectionLocation(), 1, GL_FALSE, value_ptr(projectionMatrix));
    ps0.RenderParticles(viewMatrix);
    ps1.RenderParticles(viewMatrix);
    ps2.RenderParticles(viewMatrix);
    ps3.RenderParticles(viewMatrix);
}

void PostProcessingPass(mat4 projectionMatrix, mat4 viewMatrix, mat4* oldViewProjectionMatrix)
{
    renderer.Reset();
    
    /* MOTION BLUR */
    if (activeMotionBlur) {
        
        motionBlurShader.UseShader();
        renderer.Clear();
        
        motionBlurShader.SetMotionBlurTextures(0, 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, renderer.GetTexture());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, renderer.GetDepthTexture());
        glUniform1i(motionBlurShader.GetShowHalfScreenOnlyLocation(), showHalfScreenOnly);
        
        *oldViewProjectionMatrix = projectionMatrix * viewMatrix;
        
        glUniformMatrix4fv(motionBlurShader.GetViewProjectionInverseMatrix(), 1, GL_FALSE, value_ptr(inverse(projectionMatrix * viewMatrix)));
        
        screenQuad.RenderTexture(renderer.GetTexture());
        
        glUniformMatrix4fv(motionBlurShader.GetPreviousViewProjectionMatrix(), 1, GL_FALSE, value_ptr(*oldViewProjectionMatrix));
    }
    
    /* PASSTHROUGH */
    else
    {
        renderer.Clear();
        passthrough.UseShader();
        screenQuad.RenderTexture(renderer.GetTexture());
    }
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
    
    // Clear the window
    renderer.Clear();
    glEnable(GL_DEPTH_TEST);
    
    vec3 cameraPos = camera.getCameraPosition();
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, value_ptr(projectionMatrix));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, value_ptr(viewMatrix));
    glUniform3f(uniformEyePosition, cameraPos.x, cameraPos.y, cameraPos.z);
    mat4 matrix = mainLight.CalculateLightTransform();
    
    shaderList[0].SetDirectionalLight(&mainLight);
    shaderList[0].SetDirectionalLightTransform(&matrix);
    mainLight.GetShadowMap()->Read(GL_TEXTURE2);
    shaderList[0].SetTexture(1);
    shaderList[0].SetDirectionalShadowMap(2);
    
    shaderList[0].SetPointLights(pointLights, pointLightCount, 3, 0);
    shaderList[0].SetSpotLights(spotLights, spotLightCount, 3 + pointLightCount, pointLightCount);
    
    // Let one spotlight follow the camera
    glm::vec3 handPosition = cameraPos;
    handPosition.y -= 0.3f;
    spotLights[0].SetFlash(handPosition, camera.getCameraDirection());
    
    RenderScene();
}

int main() {
    
    mainWindow = Window(1366, 768);
    mainWindow.Initialise();
    
    renderer = Renderer();
    renderer.Init(mainWindow.getBufferWidth(), mainWindow.getBufferHeight());
    
    CreateObjects();
    CreateShaders();
    
    ps0 = ParticleSystem(1000);
    ps0.Init(vec3(1.3f, 0.25f, 1.8f));
    
    ps1 = ParticleSystem(1000);
    ps1.Init(vec3(1.3f, 0.25f, 1.13f));
    
    ps2 = ParticleSystem(1000);
    ps2.Init(vec3(1.3f, 0.66f, 1.8f));
    
    ps3 = ParticleSystem(1000);
    ps3.Init(vec3(1.3f, 0.66f, 1.13f));
    
    screenQuad = Quad();
    screenQuad.Init();
    
    camera = Camera(vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 5.0f, 0.5f);
    
    brickTexture = Texture((char*)"Textures/brick.png");
    brickTexture.LoadTextureA();
    dirtTexture = Texture((char*)"Textures/dirt.png");
    dirtTexture.LoadTextureA();
    plainTexture = Texture((char*)"Textures/plain.png");
    plainTexture.LoadTextureA();
    grassTexture = Texture((char*)"Textures/grass.jpg");
    grassTexture.LoadTexture();
    particleTexture = Texture((char*)"Textures/white_gradient_circle2.png");
    particleTexture.LoadTextureA();
    
    shinyMaterial = Material(4.0f, 156.0f);
    dullMaterial = Material(0.3f, 4.0f);
    
    xwing = Model();
    xwing.LoadModel("Models/x-wing.obj");
    
    blackHawk = Model();
    blackHawk.LoadModel("Models/uh60.obj");
    
    deLorean = Model();
    deLorean.LoadModel("Models/DeLorean.obj");
    
    ball = Model();
    ball.LoadModel("Models/3d-model.obj");
    
    mainLight = DirectionalLight(2048, 2048,
                                 1.0f, 1.0f, 1.0f,
                                 0.0f, 0.1f,
                                 0.0f, -15.0f, -10.0f);
    
    
    pointLights[0] = PointLight(512, 512,
                                0.01f, 100.0f,
                                0.0f, 0.0f, 1.0f,   // Blue
                                0.3f, 1.0f,
                                0.0f, 0.0f, 0.0f,
                                0.3f, 0.2f, 0.1f);
    pointLightCount++;
    
    
    pointLights[1] = PointLight(512, 512,
                                0.01f, 100.0f,
                                0.0f, 1.0f, 0.0f,   // Green
                                0.3f, 0.8f,
                                -4.0f, 2.0f, 3.0f,
                                0.3f, 0.1f, 0.1f);
    pointLightCount++;
    
    /*
    spotLights[0] = SpotLight(2048, 2048,
                              0.01f, 1000.0f,
                              1.0f, 1.0f, 1.0f,
                              0.0f, 2.0f,
                              0.0f, 0.0f, 0.0f,
                              0.0f, -1.0f, 0.0f,
                              1.0f, 0.0f, 0.0f,
                              28.0f);
    spotLightCount++;
    
    spotLights[1] = SpotLight(2048, 2048,
                              0.01f, 1000.0f,
                              1.0f, 1.0f, 1.0f,
                              0.0f, 1.0f,
                              0.0f, -1.5f, 0.0f,
                              -100.0f, -1.0f, 0.0f,
                              1.0f, 0.0f, 0.0f,
                              20.0f);
    spotLightCount++;
    */
    mat4 projection = perspective(45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.01f, 1000.f);
   
    mat4 viewMatrix = camera.calculateViewMatrix();
    mat4 oldViewProjectionMatrix = projection * viewMatrix;
    
    motionBlurShader.UseShader();
    glUniformMatrix4fv(motionBlurShader.GetPreviousViewProjectionMatrix(), 1, GL_FALSE, value_ptr(oldViewProjectionMatrix));
    glUseProgram(0);
    
    while ( !mainWindow.getShouldClose() )
    {
        // Check errors
        checkGLError();
        
        // Timing
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;
        
        blueLightPos.x -= (deltaTime / 5.f);
        pointLights[0].SetPosition(blueLightPos);
        
        blackHawkAngle += deltaTime * 3.f;
        if (blackHawkAngle > 360.0f) blackHawkAngle = 0.1f;
        
        // Input
        glfwPollEvents( );
        camera.keyControl(deltaTime, mainWindow.getKeys());
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
        viewMatrix = camera.calculateViewMatrix();
        if (mainWindow.getKeys()[GLFW_KEY_P]) {
            mainWindow.getKeys()[GLFW_KEY_P] = false;
            activeParticles = !activeParticles;
        }
        if (mainWindow.getKeys()[GLFW_KEY_O]) {
            mainWindow.getKeys()[GLFW_KEY_O] = false;
            activeMotionBlur = !activeMotionBlur;
        }
        if (mainWindow.getKeys()[GLFW_KEY_I]) {
            mainWindow.getKeys()[GLFW_KEY_I] = false;
            showHalfScreenOnly = !showHalfScreenOnly;
        }
        if (mainWindow.getKeys()[GLFW_KEY_L]) {
            mainWindow.getKeys()[GLFW_KEY_L] = false;
            activeOmniShadowPass = !activeOmniShadowPass;
        }
            
        /* Directional ShadowMap Pass */
        DirectionalShadowMapPass(&mainLight);
        
        /* Omni-Directional ShadowMap Pass */
        if (activeOmniShadowPass) {
            for (GLuint i = 0; i < pointLightCount; i++)
            {
                OmniShadowMapPass(&pointLights[i]);
            }
            for (GLuint i = 0; i < spotLightCount; i++)
            {
                OmniShadowMapPass(&spotLights[i]);
            }
        }
        
        /* Regular render pass */
        renderer.RenderToTexture();
        RenderPass(projection, viewMatrix);
        
        /* Particles */
        if (activeParticles) ParticleRenderingPass(projection, viewMatrix);
        
        /* Post FX */
        PostProcessingPass(projection, viewMatrix, &oldViewProjectionMatrix);
        /*
        renderer.Reset();
        passthrough.UseShader();
        screenQuad.RenderTexture(  pointLights[0].GetShadowMap()->GetTexture()  );
        */
        
        /*glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glEnable(GL_DEPTH_TEST);
        chrAbrShader.UseShader();
        screenQuad.RenderTexture(renderer.GetTexture());*/
        
        glUseProgram(0);
        // Swap buffers
        mainWindow.swapBuffers();
        
    }
    
    glfwTerminate( );
    
    return 0;
}
