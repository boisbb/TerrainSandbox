#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <filesystem>
#include <sys/stat.h>
#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "glm/glm.hpp"

#include "Mesh.h"


class Model
{
public:
    Model();
    Model(std::string model, std::string resources = "res/textures/");
    ~Model();

    void Draw(Shader& shader, Camera& camera, glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 translate = glm::vec3(0.0f, 0.0f, 0.0f));
    void AddMeshTexture(Texture& texture, int position);
    void AddMesh(Mesh& mesh);
    Mesh GetMesh(int index);
    
private:
    std::string model_path;
    std::string resources_path;
    std::vector<Mesh> meshes;

    std::vector<Texture> loadedTex;

    void processMesh(aiMesh *mesh, const aiScene *scene, aiMatrix4x4 accTransform);
    void processNode(aiNode *node, const aiScene *scene, aiMatrix4x4 accTransform);
    bool loadModel();
};