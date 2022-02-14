#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <math.h>
#include <iostream>

class ProceduralGeneration {
public:
    ProceduralGeneration();
    ProceduralGeneration(int gridX, int gridZ, int vertexCount);

    float GenerateHeight(int x, int z);
    float GetInterpolatedNoise(float x, float z);
    float interpolate(float a, float b, float blend);
    float GetSmoothNoise(int x, int z);
    float GetNoise(int x, int z);

private:
    float amplitude = 100.0f;
    int octaves = 3;
    float roughness = 0.2f;

    std::random_device rd;
    std::default_random_engine e;
    std::uniform_real_distribution<double> dist;


    int seed = rand() % 1000000000;
    int xOffset = 0;
    int zOffset = 0;
};