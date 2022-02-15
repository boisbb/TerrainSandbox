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

    std::vector<Vertex> GenerateTerrainVertices();
    std::vector<unsigned int> GenerateTerrainIndices();
    void GenerateTerrainModel();
    void GenerateSubsurfaceCubes(int offsetX, int offsetZ, int surfaceHeight);
    void Draw(Shader& shader, Camera& camera);
    int GetCubeHeight(int offsetX, int offsetZ);
    int GetYCoord(int offsetX, int offsetZ);
    int GetSize();

private:
    float size;
    float maxHeight = 40;
    float maxPixColour = 256 * 256 * 256;
    bool procedural = false;
    bool heightMapped = false;
    bool flat = false;

    float x;
    float z;

    std::unique_ptr<Model> terrainModel;
    std::unique_ptr<Model> subsurfaceModel;
    std::unique_ptr<ProceduralGeneration> heightGenerator;
    std::string texturePath;
    unsigned char* terrainHeightMap;
    int mapHeight, mapWidth, mapBPP;
    std::vector<std::vector<int>> heights;
    std::vector<Vertex> additionalVertices;
    std::vector<unsigned int> additionalIndices;
};