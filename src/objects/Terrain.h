#pragma once

#include <string>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <cmath>

#include "Model.h"
#include "ProceduralGeneration.h"

#include "stb_image/stb_image.h"

class Terrain {
public:
    Terrain();
    Terrain(int gridX, int gridZ, float _size, int _vertexCount);
    Terrain(int gridX, int gridZ, float _size, int _vertexCount, std::string _texturePath, bool _procedural = false);
    Terrain(int gridX, int gridZ, float _size, std::string _texturePath, std::string _heightMapPath);

    std::vector<Vertex> GenerateTerrainVertices();
    std::vector<unsigned int> GenerateTerrainIndices();
    void GenerateTerrainModel();
    void Draw(Shader& shader, Camera& camera);
    float GetVertexHeight(int x, int z);
    float GetYCoord(int worldX, int worldZ);
    glm::vec3 GetNormals(int x, int z);

    // -- Procedural terrain generation stuff
    

    float barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos);
    //std::vector<Mesh> meshes;

private:
    float size;
    int vertexCount;
    float maxHeight = 40;
    float maxPixColour = 256 * 256 * 256;
    bool procedural = false;
    bool heightMapped = false;
    bool flat = false;

    float x;
    float z;

    std::unique_ptr<Model> terrainModel;
    std::unique_ptr<ProceduralGeneration> heightGenerator;
    std::string texturePath;
    unsigned char* terrainHeightMap;
    int mapHeight, mapWidth, mapBPP;
    std::vector<std::vector<float>> heights;

};
