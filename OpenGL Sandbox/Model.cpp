//
//  Model.cpp
//  OpenGL Sandbox
//
//  Created by Lars Andersson on 2020-05-25.
//  Copyright © 2020 Lars Andersson. All rights reserved.
//

#include "Model.hpp"

Model::Model()
{
    
}

Model::~Model()
{
    
}

void Model::LoadModel(const std::string& fileName)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
    
    if ( !scene )
    {
        printf("Model '(%s)' failed to load: %s\n", fileName.c_str(), importer.GetErrorString());
        return;
    }
    
    LoadNode(scene->mRootNode, scene);
    
    LoadMaterials(scene);
}

void Model::RenderModel()
{
    for (unsigned int i = 0; i < meshList.size(); i++)
    {
        unsigned int materialIndex = meshToTex[i];
        
        if (materialIndex < textureList.size() && textureList[materialIndex])
        {
            textureList[materialIndex]->UseTexture();
        }
        
        meshList[i]->RenderMesh();
    }
}

void Model::ClearModel()
{
    for (unsigned int i = 0; i < meshList.size(); i++)
    {
        if (meshList[i])
        {
            delete meshList[i];
            meshList[i] = nullptr;
        }
    }
    
    for (unsigned int i = 0; i < textureList.size(); i++)
    {
        if (textureList[i])
        {
            delete textureList[i];
            textureList[i] = nullptr;
        }
    }
}

void Model::LoadNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
    }
    
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        LoadNode(node->mChildren[i], scene);
    }
}

void Model::LoadMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<GLfloat> vertices;
    std::vector<unsigned int> indices;
    
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });
        if (mesh->mTextureCoords[0])
        {
            // Only works for one texture atm.
            vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
        } else {
            vertices.insert(vertices.end(), { 0.0f, 0.0f });
        }
        // Inverted normals, should probably invert the normals in the shader instead!
        vertices.insert(vertices.end(), { -mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z });
    }
    
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }
    
    Mesh* newMesh = new Mesh();
    newMesh->CreateMesh( &vertices[0], &indices[0], (unsigned int)vertices.size(), (unsigned int)indices.size());
    meshList.push_back(newMesh);
    meshToTex.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterials(const aiScene* scene)
{
    textureList.resize(scene->mNumMaterials);
    
    for (unsigned int i = 0; i < scene->mNumMaterials; i++)
    {
        aiMaterial* material = scene->mMaterials[i];
        
        textureList[i] = nullptr;
        
        // If there actually is a material
        if (material->GetTextureCount(aiTextureType_DIFFUSE))
        {
            aiString path;
            if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
            {
                // Is this a fix for windows only?
                int idx = (int) std::string(path.data).rfind("\\");
                std::string filename = std::string(path.data).substr(idx+1);
                
                std::string texPath = std::string("Textures/") + filename;
                
                textureList[i] = new Texture(texPath.c_str());
                
                if (!textureList[i]->LoadTexture())
                {
                    printf("Failed to load texture at: %s\n", texPath.c_str());
                    delete textureList[i];
                    textureList[i] = nullptr;
                }
            }
        }
        
        if (!textureList[i])
        {
            textureList[i] = new Texture("Textures/plain.png");
            textureList[i]->LoadTextureA();
        }
    }
}
