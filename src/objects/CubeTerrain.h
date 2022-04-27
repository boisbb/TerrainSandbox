#pragma once

#include <string>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <cmath>

#include "Model.h"
#include "ProceduralGeneration.h"

class CubeTerrain {
public:
    CubeTerrain();
    CubeTerrain(int gridX, int gridZ, float _size);
    CubeTerrain(int gridX, int gridZ, float _size, std::string _texturePath, bool _procedural = false);
    CubeTerrain(int gridX, int gridZ, std::string _texturePath, std::string _heightMapPath);

    int GetBlockMaterial(int height, glm::vec3 normal, int offset);
    std::vector<Vertex> GenerateTerrainVertices();
    std::vector<unsigned int> GenerateTerrainIndices();
    void GenerateTerrainModel();
    void GenerateSubsurfaceCubes(int offsetX, int offsetZ, int surfaceHeight, int bottom);
    void GenerateBottomCube(int offsetX, int offsetZ);
    void Draw(Shader& shader, Camera& camera);
    int GetCubeHeight(int offsetX, int offsetZ);
    int GetNeighbourMinHeight(int offsetX, int offsetZ, int height);
    int GetYCoord(int offsetX, int offsetZ);
    int GetSize();

private:
    float size;
    float maxHeight = 200;
    float maxPixColour = 256 * 256 * 256;
    bool procedural = false;
    bool heightMapped = false;
    bool flat = false;
    int minDepth = 0;

    // Materials
    glm::vec3 grass = glm::vec3(0, 1, 2);
    unsigned short dirt = 2;
    unsigned short sand = 16;
    unsigned short stone = 3;

    // Material levels
    int stoneLevel = 12;
    int grassLevel = -15;
    int maxMaterialOffset = 10;



    float x;
    float z;

    std::unique_ptr<Model> terrainModel;
    std::unique_ptr<Model> subsurfaceModel;
    std::vector<Mesh> cubes;
    std::unique_ptr<ProceduralGeneration> heightGenerator;
    std::string texturePath;
    unsigned char* terrainHeightMap;
    int mapHeight, mapWidth, mapBPP;
    std::vector<std::vector<int>> heights;
    std::vector<Vertex> additionalVertices;
    std::vector<unsigned int> additionalIndices;
};