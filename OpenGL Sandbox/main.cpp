#define STB_IMAGE_IMPLEMENTATION

#if DEBUG
#define glError() checkGlError(__FILE__,__LINE__)
#else
#define glError()
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

#include "GameHandler.hpp"

#include "Renderer.hpp"
#include "GBuffer.hpp"

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
Shader passthroughShader;
Shader motionBlurShader;
Shader chrAbrShader;
Shader prePassShader;
Shader ssaoShader;
Shader ssaoBlurShader;

Renderer renderer;
GBuffer gBuffer;

ParticleSystem ps0;
ParticleSystem ps1;
ParticleSystem ps2;
ParticleSystem ps3;

Quad screenQuad;

GameHandler gameHandler;

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

mat4 tri1_matrix = mat4(1.0f);
mat4 tri2_matrix = mat4(1.0f);
mat4 floor_matrix = mat4(1.0f);
mat4 xwing_matrix = mat4(1.0f);
mat4 blackHawk_matrix = mat4(1.0f);
mat4 deLorean_matrix = mat4(1.0f);
mat4 ball1_matrix = mat4(1.0f);
mat4 ball2_matrix = mat4(1.0f);
mat4 ball3_matrix = mat4(1.0f);
mat4 ball4_matrix = mat4(1.0f);

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];
unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
GLfloat blackHawkAngle = 0.0f;
bool finalRenderStage = false;

bool activeOmniShadowPass = false;
bool activeMotionBlur = false;
bool activeParticles  = false;
bool showHalfScreenOnly = false;

vec3 blueLightPos  = vec3(0.0f);
vec3 scoreLightPos = vec3(-4.0f, 0.0f, 3.0f);

#if DEBUG
void checkGlError(const char* file, int line) {
    GLenum err(glGetError());

    while (err != GL_NO_ERROR) {
        string error;
        switch (err) {
            case GL_INVALID_OPERATION:
                error = "INVALID_OPERATION";
                break;
            case GL_INVALID_ENUM:
                error = "INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error = "INVALID_VALUE";
                break;
            case GL_OUT_OF_MEMORY:
                error = "OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "INVALID_FRAMEBUFFER_OPERATION";
                break;
            default:
                error = "Unknown error";
                break;
        }
        std::cout << file << " " << line << ": GL_" << error << std::endl;
        err = glGetError();
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
    
    passthroughShader = Shader();
    passthroughShader.CreateFromFiles("Shaders/Passthrough/passthrough.vert", "Shaders/Passthrough/simpleTexture.frag");
    
    motionBlurShader = Shader();
    motionBlurShader.CreateFromFiles("Shaders/MotionBlur/motionBlur.vert", "Shaders/MotionBlur/motionBlur.frag");
    
    chrAbrShader = Shader();
    chrAbrShader.CreateFromFiles("Shaders/CHRABR/passthrough.vert", "Shaders/CHRABR/chromatic_aberration.frag");
    
    prePassShader = Shader();
    prePassShader.CreateFromFiles("Shaders/DeferredRendering/prePass.vert", "Shaders/DeferredRendering/prePass.frag");
    
    ssaoShader = Shader();
    ssaoShader.CreateFromFiles("Shaders/SSAO/SSAOshader.vert", "Shaders/SSAO/SSAOshader.frag");
    
    ssaoBlurShader = Shader();
    ssaoBlurShader.CreateFromFiles("Shaders/SSAO/SSAOshader.vert", "Shaders/SSAO/SSAOblurShader.frag");
}

void RenderScene()
{
    
    // TRI1
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(tri1_matrix));
    brickTexture.UseTexture();
    if (finalRenderStage) shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[0]->RenderMesh();
    
    // TRI2
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(tri2_matrix));
    dirtTexture.UseTexture();
    if (finalRenderStage) dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[1]->RenderMesh();
    
    // FLOOR
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(floor_matrix));
    dirtTexture.UseTexture();
    if (finalRenderStage) dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    meshList[2]->RenderMesh();

    // XWING
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(xwing_matrix));
    if (finalRenderStage) shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    xwing.RenderModel();

    // BLACKHAWK
    /*
    model = rotate(model, -blackHawkAngle * toRadians, vec3(0.0f, 1.0f, 0.0f));
    model = translate(model, vec3(-7.0f, 2.0f, 0.0f));
    model = scale(model, vec3(0.4f, 0.4f, 0.4f));
    model = rotate(model, -15.0f * toRadians, vec3(0.0f, 0.0f, 1.0f));
    model = rotate(model, -90.0f * toRadians, vec3(1.0f, 0.0f, 0.0f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model));
    if (finalRenderStage) shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    blackHawk.RenderModel();
    */
    
    // DELOREAN
    
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(gameHandler.GetModelMatrix()));
    if (finalRenderStage) shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    deLorean.RenderModel();
    
    // BALL1
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(ball1_matrix));
    if (finalRenderStage) shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    ball.RenderModel();
    
    // BALL2
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(ball2_matrix));
    if (finalRenderStage) dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    ball.RenderModel();
    
    // LIGHTBALL 1 (BLUE)
    mat4 model = mat4(1.0f);
    model = translate(model, blueLightPos);
    model = scale(model, vec3(0.002f, 0.002f, 0.002f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model));
    if (finalRenderStage) shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    ball.RenderModel();
    
    // LIGHTBALL 2 (SCORE)
    model = mat4(1.0f);
    model = translate(model, pointLights[1].GetPosition());
    model = scale(model, vec3(0.002f, 0.002f, 0.002f));
    glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model));
    if (finalRenderStage) shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
    ball.RenderModel();
    
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

void OmniShadowMapPass(PointLight* light)
{
    omniShadowShader.UseShader();
    
    light->GetShadowMap()->Write();
    glClear(GL_DEPTH_BUFFER_BIT);
    
    uniformModel = omniShadowShader.GetModelLocation();
    uniformOmniLightPos = omniShadowShader.GetOmniLightPosLocation();
    uniformFarPlane = omniShadowShader.GetFarPlaneLocation();
    
    vec3 lightPosition = light->GetPosition();
    glUniform3f(uniformOmniLightPos, lightPosition.x, lightPosition.y, lightPosition.z);
    glUniform1f(uniformFarPlane, light->GetFarPlane());
    omniShadowShader.SetLightMatrices(light->CalculateLightTransform());
    
    RenderScene();
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ParticleRenderingPass(mat4* projectionMatrix, mat4* viewMatrix)
{
    ps0.ProcessParticles(deltaTime);
    ps1.ProcessParticles(deltaTime);
    ps2.ProcessParticles(deltaTime);
    ps3.ProcessParticles(deltaTime);
    particleShader.UseShader();
    particleTexture.UseTexture();
    glUniform1i(particleShader.GetTheTextureLocation(), 1);
    glUniformMatrix4fv(particleShader.GetProjectionLocation(), 1, GL_FALSE, value_ptr(*projectionMatrix));
    ps0.RenderParticles(viewMatrix);
    ps1.RenderParticles(viewMatrix);
    ps2.RenderParticles(viewMatrix);
    ps3.RenderParticles(viewMatrix);
}

void PostProcessingPass(mat4* projectionView, mat4* oldViewProjectionMatrix)
{
    renderer.Reset();
    
    /* MOTION BLUR */
    if (activeMotionBlur) {
        
        motionBlurShader.UseShader();
        renderer.Clear();
        
        motionBlurShader.SetMotionBlurTextures(1, 2);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, renderer.GetTexture());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gBuffer.GetDepthTexture());
        glUniform1i(motionBlurShader.GetShowHalfScreenOnlyLocation(), showHalfScreenOnly);
        
        glUniform1i(motionBlurShader.GetSSAOtextureLocation(), 3);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, renderer.GetSSAOTexture());
        
        mat4 viewProj = *projectionView;
        *oldViewProjectionMatrix = viewProj;
        
        glUniformMatrix4fv(motionBlurShader.GetViewProjectionInverseMatrix(), 1, GL_FALSE, value_ptr(inverse(viewProj)));
        
        screenQuad.RenderTexture(renderer.GetTexture());
        
        glUniformMatrix4fv(motionBlurShader.GetPreviousViewProjectionMatrix(), 1, GL_FALSE, value_ptr(*oldViewProjectionMatrix));
    }
    
    /* PASSTHROUGH */
    else
    {
        renderer.Clear();
        passthroughShader.UseShader();
        screenQuad.RenderTexture( renderer.GetTexture() );
    }
}

void RenderPass(mat4* projectionMatrix, mat4* viewMatrix)
{
    
    shaderList[0].UseShader();
    
    uniformModel = shaderList[0].GetModelLocation();
    uniformProjection = shaderList[0].GetProjectionLocation();
    uniformView = shaderList[0].GetViewLocation();
    uniformEyePosition = shaderList[0].GetEyePositionLocation();
    uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
    uniformShininess = shaderList[0].GetShininessLocation();
    glUniform1i(shaderList[0].GetUseOmniShadow(), activeOmniShadowPass);
    
    mainWindow.resetViewport();
    
    // Clear the window
    renderer.Clear();
    glEnable(GL_DEPTH_TEST);
    
    vec3 cameraPos = camera.getCameraPosition();
    glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, value_ptr(*projectionMatrix));
    glUniformMatrix4fv(uniformView, 1, GL_FALSE, value_ptr(*viewMatrix));
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
    
    finalRenderStage = true;
    RenderScene();
    finalRenderStage = false;
}

int main() {
    
    mainWindow = Window(1366, 768);    // 1920x1080, 1366x768
    mainWindow.Initialise();
    
    gameHandler = GameHandler();
    
    CreateObjects();
    CreateShaders();
    
    renderer = Renderer();
    renderer.Init(mainWindow.getBufferWidth(), mainWindow.getBufferHeight());
    
    gBuffer = GBuffer();
    gBuffer.Init(mainWindow.getBufferWidth(), mainWindow.getBufferHeight(), &prePassShader);
    
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
    
    tri1_matrix = translate(tri1_matrix, vec3(0.0f, 0.0f, -2.5f));
    tri2_matrix = translate(tri2_matrix, vec3(0.0f, 4.0f, -2.5f));
    floor_matrix = translate(floor_matrix, vec3(0.0f, -2.0f, 0.0f));
    xwing_matrix = translate(xwing_matrix, vec3(-7.0f, 0.0f, 10.0f));
    xwing_matrix = scale(xwing_matrix, vec3(0.006f, 0.006f, 0.006f));
    ball1_matrix = translate(ball1_matrix, vec3(-3.0f, -1.7f, 0.0f));
    ball1_matrix = scale(ball1_matrix, vec3(0.005f, 0.005f, 0.005f));
    ball2_matrix = translate(ball2_matrix, vec3(-3.0f, -1.7f, 1.25f));
    ball2_matrix = scale(ball2_matrix, vec3(0.005f, 0.005f, 0.005f));
    
    
    /*
    blackHawk = Model();
    blackHawk.LoadModel("Models/uh60.obj");
    */
    
    deLorean = Model();
    deLorean.LoadModel("Models/DeLorean.obj");
    
    ball = Model();
    ball.LoadModel("Models/3d-model.obj");
    
    mainLight = DirectionalLight(1024, 1024,
                                 1.0f, 1.0f, 1.0f,
                                 0.1f, 0.1f,
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
                                -4.0f, 0.0f, 3.0f,
                                0.3f, 0.1f, 0.1f);
    pointLightCount++;
    
    /*
    spotLights[0] = SpotLight(512, 512,
                              0.01f, 1000.0f,
                              1.0f, 1.0f, 1.0f,
                              0.0f, 2.0f,
                              0.0f, 0.0f, 0.0f,
                              0.0f, -1.0f, 0.0f,
                              1.0f, 0.0f, 0.0f,
                              28.0f);
    spotLightCount++;
    
    spotLights[1] = SpotLight(512, 512,
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
    
    ssaoShader.UseShader();
    glUniform1i(ssaoShader.GetPositionTextureLocation(), 1);
    glUniform1i(ssaoShader.GetNormalTextureLocation(), 2);
    glUniform1i(ssaoShader.GetNoiseTextureLocation(), 3);
    glUniform1f(ssaoShader.GetScreenWidthLocation(), mainWindow.getBufferWidth());
    glUniform1f(ssaoShader.GetScreenHeightLocation(), mainWindow.getBufferHeight());
    glUniform3fv(ssaoShader.GetSSAOsamplesLocation(), hemisphereSamples, value_ptr(*renderer.GetSSAOsamples()));
    glUseProgram(0);
    
    GLuint uniformCameraPosSSAO = ssaoShader.GetCameraPositionLocation();
    GLuint uniformCameraDirSSAO = ssaoShader.GetCameraDirectionLocation();
    GLuint uniformViewProjSSAO = ssaoShader.GetViewProjectionLocation();
    
    while ( !mainWindow.getShouldClose() )
    {
        // Check errors
        glError();
        
        // Timing
        GLfloat now = glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;
        
        gameHandler.Update(&mainWindow, &pointLights[1], deltaTime);
        
        blueLightPos.x -= (deltaTime / 5.f);
        pointLights[0].SetPosition(blueLightPos);   
        
        blackHawkAngle += deltaTime * 3.f;
        if (blackHawkAngle > 360.0f) blackHawkAngle = 0.1f;
        
        // Input
        glfwPollEvents( );
        mainWindow.updateGamepad();
        camera.keyControl(deltaTime, mainWindow.getKeys());
        camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
        viewMatrix = camera.calculateViewMatrix();
        mat4 projectionView = projection * viewMatrix;
        vec3 cameraPos = camera.getCameraPosition();
        vec3 cameraDir = camera.getCameraDirection();
        
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
        if (mainWindow.getKeys()[GLFW_KEY_K]) {
            mainWindow.getKeys()[GLFW_KEY_K] = false;
            activeOmniShadowPass = !activeOmniShadowPass;
        }
        if (mainWindow.getKeys()[GLFW_KEY_L]) {
            mainWindow.getKeys()[GLFW_KEY_L] = false;
            spotLights[0].Toggle();
        }
        if (mainWindow.getKeys()[GLFW_KEY_V]) {
            mainWindow.getKeys()[GLFW_KEY_V] = false;
            camera.setTarget(gameHandler.GetPlayer());
        }
        
        /* PrePass for deferred rendering */
        gBuffer.Render(&projection, &viewMatrix);
        uniformModel = prePassShader.GetModelLocation();
        RenderScene();
        gBuffer.Reset();
        
        /* SSAO pass */
        renderer.RenderSSAO(gBuffer.GetPositionTexture(), gBuffer.GetNormalsTexture());
        ssaoShader.UseShader();
        glUniform3f(uniformCameraPosSSAO, cameraPos.x, cameraPos.y, cameraPos.z);
        glUniform3f(uniformCameraDirSSAO, cameraDir.x, cameraDir.y, cameraDir.z);
        glUniformMatrix4fv(uniformViewProjSSAO, 1, GL_FALSE, value_ptr(projectionView));
        screenQuad.RenderQuad();
        renderer.Reset();
        glUseProgram(0);
        
        renderer.RenderBlurredSSAO();
        ssaoBlurShader.UseShader();
        glUniform1i(ssaoBlurShader.GetTheTextureLocation(), 1);
        screenQuad.RenderQuad();
        renderer.Reset();
        glUseProgram(0);
            
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
        RenderPass(&projection, &viewMatrix);
        
        /* Particles */
        if (activeParticles) ParticleRenderingPass(&projection, &viewMatrix);
        
        /* Post FX */
        PostProcessingPass(&projectionView, &oldViewProjectionMatrix);
        
        GLfloat theTime = glfwGetTime();
        
        renderer.Reset();
        glUseProgram(0);
        // Swap buffers
        mainWindow.swapBuffers();
        /*
        printf("%f\n", (glfwGetTime() - theTime) * 1000.0f);
        theTime = glfwGetTime();
        */
    }
    
    glfwTerminate( );
    
    return 0;
}
