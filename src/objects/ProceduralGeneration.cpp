#include "ProceduralGeneration.h"


ProceduralGeneration::ProceduralGeneration() 
{
    
}

ProceduralGeneration::ProceduralGeneration(int gridX, int gridZ, int vertexCount) 
{
    xOffset = gridX * (vertexCount - 1);
    zOffset = gridZ * (vertexCount - 1);

    //std::srand(time(NULL));
    //std::seed_seq ssq{rd()};

    std::uniform_real_distribution<> distr(0.0f, 1.0f);
    dist = distr;
}

ProceduralGeneration::ProceduralGeneration(int gridX, int gridZ) 
{
    xOffset = gridX * 2;
    zOffset = gridZ * 2;

    std::uniform_real_distribution<> distr(0.0f, 1.0f);
    dist = distr;
}

float ProceduralGeneration::GenerateHeight(int x, int z) 
{
    //float total = GetInterpolatedNoise(x / 4.0f, z / 4.0f) * amplitude;
    
    float total = 0;
    
    float d = (float)pow(2, octaves);
    for(int i = 0; i < octaves; i++) {
        float freq = (float)(pow(2, i) / d);
        float amp = (float)pow(roughness, i) * amplitude;
        total += GetInterpolatedNoise((x + xOffset) * freq, (z + zOffset) * freq) * amp;
    }

    return total;
}

float ProceduralGeneration::GetInterpolatedNoise(float x, float z) 
{
    int intX = (int)x;
    int intZ = (int)z;
    float fracX = x - intX;
    float fracZ = z - intZ;

    float v1 = GetSmoothNoise(intX, intZ);
    float v2 = GetSmoothNoise(intX + 1, intZ);
    float v3 = GetSmoothNoise(intX, intZ + 1);
    float v4 = GetSmoothNoise(intX + 1, intZ + 1);
    float i1 = interpolate(v1, v2, fracX);
    float i2 = interpolate(v3, v4, fracX);
    return interpolate(i1, i2, fracZ);
}

float ProceduralGeneration::interpolate(float a, float b, float blend) 
{
    double theta = blend * M_PI;
    float f = (float)(1.0f - cos(theta)) * 0.5f;
    return a * (1.0f - f) + b * f;
}

float ProceduralGeneration::GetSmoothNoise(int x, int z) 
{
    float corners = (GetNoise(x - 1, z - 1) + GetNoise(x + 1, z - 1) + GetNoise(x - 1, z + 1) + GetNoise(x + 1, z + 1)) / 16.0f;
    float sides = (GetNoise(x - 1, z) + GetNoise(x + 1, z) + GetNoise(x, z - 1) + GetNoise(x, z + 1)) / 8.0f;
    float center = GetNoise(x, z) / 4.0f;
    return corners + sides + center;
}

float ProceduralGeneration::GetNoise(int x, int z) 
{
    //std::srand(x * 49632 + z * 325176 + seed);
    //float result = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
    std::default_random_engine eng{static_cast<long unsigned int>(x * 49632 + z * 325176 + seed)};
    std::uniform_int_distribution<> distr(0.0f, RAND_MAX);
    //std::default_random_engine eng(rd());
    //std::mt19937 mt((x * 49632 + z * 325176 + seed));
    //float result = dist(eng) * 2.0f - 1.0f;
    float result = (static_cast<float>(distr(eng)) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;;
    //std::cout << result << std::endl;
    return result;
}

float ProceduralGeneration::GetAmplitude() const
{
    return amplitude;
}
