#include <math.h>
#include <cmath>

#include "Terrain.h"

#include "vendor/stb_image/stb_image.h"

Terrain::Terrain() 
{
    terrainModel = std::make_unique<Model>();

    GenerateTerrainModel();
}

Terrain::Terrain(int gridX, int gridZ, float _size, int _vertexCount) 
    :   x(gridX), z(gridZ), size(_size), vertexCount(_vertexCount)
{
    x = gridX * size;
    z = gridZ * size;

    terrainModel = std::make_unique<Model>();
    //terrainModel = std::make_unique<Model>("../res/models/lightCube/lightCube.obj");
    GenerateTerrainModel();
}

Terrain::Terrain(int gridX, int gridZ, float _size, int _vertexCount, std::string _texturePath, bool _procedural)
    :   size(_size), vertexCount(_vertexCount), texturePath(_texturePath), flat(!_procedural), procedural(_procedural)
{
    x = gridX * size;
    z = gridZ * size;

    if (procedural)
    {
        heightGenerator = std::make_unique<ProceduralGeneration>(gridX, gridZ, vertexCount);
    }
    

    terrainModel = std::make_unique<Model>();
    GenerateTerrainModel();
}

Terrain::Terrain(int gridX, int gridZ, float _size, std::string _texturePath, std::string _heightMapPath) 
    :   size(_size), texturePath(_texturePath), heightMapped(true)
{
    x = gridX * size;
    z = gridZ * size;


    stbi_set_flip_vertically_on_load(1);
    terrainHeightMap = stbi_load(_heightMapPath.c_str(), &mapWidth, &mapHeight, &mapBPP, 3);
    vertexCount = mapHeight;
    //vertexCount = 128;
     
    terrainModel = std::make_unique<Model>();
    GenerateTerrainModel();
}

std::vector<Vertex> Terrain::GenerateTerrainVertices() 
{
    std::vector<Vertex> vertices;
    for (int i = 0; i < vertexCount; i++) {
        std::vector<float> jheights;
        for (int j = 0; j < vertexCount; j++) {
            Vertex vertex;
            jheights.push_back(GetVertexHeight(j, i));
            
            vertex.position = glm::vec3((float)j / ((float)vertexCount - 1) * size,
                                        GetVertexHeight(j, i),
                                        (float)i / ((float)vertexCount - 1) * size);
            vertex.normal = GetNormals(j, i);
            vertex.texUV = glm::vec2((float)j / ((float)vertexCount - 1),
                                     (float)i / ((float)vertexCount - 1));
            
            //std::cout<<vertex.position.x << " " <<vertex.position.z<< std::endl;
            vertices.push_back(vertex);
            
            
        }
        heights.push_back(jheights);
    }

    return vertices;
}

std::vector<unsigned int> Terrain::GenerateTerrainIndices() 
{
    std::vector<unsigned int> indices;
    for (int gz = 0; gz < vertexCount - 1; gz++) {
        for (int gx = 0; gx < vertexCount - 1; gx++) {
            int topLeft = (gz * vertexCount) + gx;
            int topRight = topLeft + 1;
            int bottomLeft = ((gz + 1) * vertexCount) + gx;
            int bottomRight = bottomLeft + 1;

            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    //for (unsigned int index : indices)
    //    std::cout << index << std::endl;

    return indices;
}



void Terrain::GenerateTerrainModel() 
{
    std::vector<Vertex> vertices = GenerateTerrainVertices();
    std::vector<unsigned int> indices = GenerateTerrainIndices();


    Mesh terrainMesh = Mesh(vertices, indices);
    Texture terrainTexture = Texture(texturePath, 1);
    terrainMesh.AddTexture(terrainTexture);
    //meshes.push_back(Mesh(vertices, indices));

    terrainModel->AddMesh(terrainMesh);
}

void Terrain::Draw(Shader& shader, Camera& camera) 
{
    terrainModel->Draw(shader, camera);
}

float Terrain::GetVertexHeight(int x, int z) 
{
    if (flat)
    {
        return 0;
    }
    else if (heightMapped) 
    {
        unsigned bytesPerPixel = 3;
        unsigned char* pixelOffset = terrainHeightMap + (x + mapHeight * z) * bytesPerPixel;
        unsigned char r = pixelOffset[0];
        unsigned char g = pixelOffset[1];
        unsigned char b = pixelOffset[2];
        int rBit = r << 16;
        int gBit = g << 8;
        int bBit = b;

        float height = rBit + gBit + bBit;
        height += maxPixColour / (float)2;
        height /= maxPixColour / (float)2;

        return height * maxHeight;
    }
    else if (procedural)
    {
        float height = heightGenerator->GenerateHeight(x, z);
        //std::cout << height << std::endl;
        return height;
    }

    return 0;
}

float Terrain::GetYCoord(int worldX, int worldZ) 
{
    float terrainX = worldX - x;
    float terrainZ = worldZ - z;

    float gridSquaresSize = size / ((float)vertexCount - 1);

    int gridX = (int)floor(terrainX / gridSquaresSize);
    int gridZ = (int)floor(terrainZ / gridSquaresSize);

    if (gridX >= vertexCount - 1 || gridZ >= vertexCount - 1 || gridX < 0 || gridZ < 0)
        return 0;
    
    float xCoord = fmodf(terrainX, gridSquaresSize) / gridSquaresSize;
    float zCoord = fmodf(terrainZ, gridSquaresSize) / gridSquaresSize;

    if (xCoord <= (1 - zCoord))
    {
        return barryCentric(glm::vec3(0, heights[gridX][gridZ], 0),
                            glm::vec3(1, heights[gridX + 1][gridZ], 0),
                            glm::vec3(0, heights[gridX][gridZ + 1], 1),
                            glm::vec2(xCoord, zCoord));
    }
    else {
        return barryCentric(glm::vec3(0, heights[gridX + 1][gridZ], 0),
                            glm::vec3(1, heights[gridX + 1][gridZ + 1], 0),
                            glm::vec3(0, heights[gridX][gridZ + 1], 1),
                            glm::vec2(xCoord, zCoord));
    }
    
}

/**
 * @brief Compute the normals based on height of neighbouring vertices.
 * 
 * @param x coord
 * @param z coord
 * @return glm::vec3 
 */
glm::vec3 Terrain::GetNormals(int x, int z) 
{
    float heightL = GetVertexHeight(x - 1, z);
    float heightR = GetVertexHeight(x + 1, z);
    float heightD = GetVertexHeight(x, z - 1);
    float heightU = GetVertexHeight(x, z + 1);
    glm::vec3 normal = glm::normalize(glm::vec3(heightL - heightR, 2.0f, heightD - heightU));
    return normal;
}


// https://www.dropbox.com/s/0md240yyc359ik3/code.txt?dl=0
float Terrain::barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos) 
{
    float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
    float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
    float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
    float l3 = 1.0f - l1 - l2;
    return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}