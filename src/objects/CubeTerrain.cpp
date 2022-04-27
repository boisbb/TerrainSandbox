#include "CubeTerrain.h"

glm::vec3 cubeCenter(1, 1, 1);

std::vector<glm::vec3> cubePositions = 
{
    glm::vec3(2, 2, 0),
    glm::vec3(0, 2, 0),
    glm::vec3(0, 2, 2),
    glm::vec3(2, 2, 2),

    glm::vec3(2, 0, 2),
    glm::vec3(2, 2, 2),
    glm::vec3(0, 2, 2),
    glm::vec3(0, 0, 2),

    glm::vec3(0, 0, 2),
    glm::vec3(0, 2, 2),
    glm::vec3(0, 2, 0),
    glm::vec3(0, 0, 0),

    glm::vec3(0, 0, 0),
    glm::vec3(2, 0, 0),
    glm::vec3(2, 0, 2),
    glm::vec3(0, 0, 2),

    glm::vec3(2, 0, 0),
    glm::vec3(2, 2, 0),
    glm::vec3(2, 2, 2),
    glm::vec3(2, 0, 2),

    glm::vec3(0, 0, 0),
    glm::vec3(0, 2, 0),
    glm::vec3(2, 2, 0),
    glm::vec3(2, 0, 0),
};

std::vector<glm::vec2> cubeUVs
{
    glm::vec2(1,1),
    glm::vec2(1,0),
    glm::vec2(0,0),
    glm::vec2(0,1),
};

std::vector<glm::vec3> cubeNormals 
{
    glm::vec3(0,1,0),
    glm::vec3(0,0,1),
    glm::vec3(-1,0,0),
    glm::vec3(0,-1,0),
    glm::vec3(1,0,0),
    glm::vec3(0,0,-1),
};


std::vector<unsigned int> cubeIndices = {
    0,1,2,
    0,2,3,

    4,5,6,
    4,6,7,

    8,9,10,
    8,10,11,

    12,13,14,
    12,14,15,

    16,17,18,
    16,18,19,

    20,21,22,
    20,22,23
};


CubeTerrain::CubeTerrain() 
{
    terrainModel = std::make_unique<Model>();
    GenerateTerrainModel();
}

CubeTerrain::CubeTerrain(int gridX, int gridZ, float _size) 
    :   size(_size)
{
    x = gridX * size;
    z = gridZ * size;

    terrainModel = std::make_unique<Model>();
    GenerateTerrainModel();
}

CubeTerrain::CubeTerrain(int gridX, int gridZ, float _size, std::string _texturePath, bool _procedural) 
    :   size(_size), texturePath(_texturePath), flat(!_procedural), procedural(_procedural)
{
    x = gridX * size;
    z = gridZ * size;

    if (procedural)
    {
        heightGenerator = std::make_unique<ProceduralGeneration>(gridX, gridZ);
        minDepth = (int)((-1) * (heightGenerator->GetAmplitude() / 2));
    }
    

    terrainModel = std::make_unique<Model>();
    subsurfaceModel = std::make_unique<Model>();
    GenerateTerrainModel();
}

CubeTerrain::CubeTerrain(int gridX, int gridZ, std::string _texturePath, std::string _heightMapPath) 
    : texturePath(_texturePath), heightMapped(true)
{
    x = gridX * size;
    z = gridZ * size;

    size = 40;


    stbi_set_flip_vertically_on_load(1);
    terrainHeightMap = stbi_load(_heightMapPath.c_str(), &mapWidth, &mapHeight, &mapBPP, 3);
    size = mapHeight;
    std::cout << size << std::endl;
    //vertexCount = 128;
     
    terrainModel = std::make_unique<Model>();
    GenerateTerrainModel();
}

int CubeTerrain::GetBlockMaterial(int height, glm::vec3 normal, int offset)
{

    if (height + offset >= stoneLevel)
    {
        return stone;
    }
    else if (height + offset >= grassLevel && height + offset < stoneLevel)
    {
        if (normal.y == 1.0)
        {
            return grass[0];
        }
        else if (normal.x != 0.0 || normal.z != 0.0)
        {
            return grass[1];
        }
        else if (normal.y == -1.0)
        {
            return grass[2];
        }
    }
    else
    {
        return sand;
    }
}

std::vector<Vertex> CubeTerrain::GenerateTerrainVertices() 
{
    std::vector<Vertex> vertices;

    for (int i = 0; i < size; i++) {
        std::vector<int> jheights;
        for (int j = 0; j < size; j++) {
            //std::cout << "before " << i << " " << j <<std::endl;
            int height = GetCubeHeight(j, i);
            jheights.push_back(height);
            
            int materialOffset = std::rand() % maxMaterialOffset; 
            for (int k = 0; k < 24; k++) {
                Vertex vertex;
                vertex.position = cubePositions[k] + glm::vec3(j * 2, height, i * 2);
                vertex.normal = cubeNormals[k / 4];
                vertex.texUV = cubeUVs[k % 4];
                vertex.meshCenterHeight = height;
                vertex.textureIndex = GetBlockMaterial(height, vertex.normal, materialOffset);
                vertices.push_back(vertex);


                /*
                std::cout << vertex.position.y << std::endl;
                std::cout << vertex.position.x << " " << vertex.position.y << " " << vertex.position.z << std::endl;
                std::cout << vertex.normal.x << " " << vertex.normal.y << " " << vertex.normal.z << std::endl;
                std::cout << vertex.texUV.x << " " << vertex.texUV.y << std::endl;
                std::cout << "----" << std::endl;
                */
            }

            //std::cout << "Cube end" << std::endl;



            if ((i != 0 && i != (size - 1)) && (j != 0 && j != (size - 1)))
            {
                //std::cout << i << " " << j <<std::endl;
                int min = GetNeighbourMinHeight(j, i, height);
                if (abs(height - min) > 2) {
                    GenerateSubsurfaceCubes(j, i, height, min);
                }
                GenerateBottomCube(j, i);
                continue;
            }

            GenerateSubsurfaceCubes(j, i, height, minDepth);
            //std::cout << "after" << std::endl;
        }
        heights.push_back(jheights);

    }

    return vertices;
}

std::vector<unsigned int> CubeTerrain::GenerateTerrainIndices() 
{
    std::vector<unsigned int> indices;
    for (int i = 0; i < size * size; i++) {
        for (int j = 0; j < cubeIndices.size(); j++) {
            indices.push_back(cubeIndices[j] + i * 24);
            // std::cout << cubeIndices[j] + i * 24 << std::endl;
        }
    }

    return indices;
}


void CubeTerrain::GenerateTerrainModel() 
{
    std::vector<Vertex> vertices = GenerateTerrainVertices();
    std::cout << "vertices generated" << std::endl;
    std::vector<unsigned int> indices = GenerateTerrainIndices();
    std::cout << "indices generated" << std::endl;

    /*
    for (unsigned int index : indices) {
        std::cout << index << std::endl;
    }*/

    //vertices.insert(vertices.end(), additionalVertices.begin(), additionalVertices.end());
    //indices.insert(indices.end(), additionalIndices.begin(), additionalIndices.end());

    Mesh terrainMesh = Mesh(vertices, indices);
    Mesh subsurfaceMesh = Mesh(additionalVertices, additionalIndices);

    // MANUALLY ENTERED SLOT WATCH OUT FOR THIS TODO
    Texture terrainTexture(texturePath, 1, 16, false);
    terrainMesh.AddTexture(terrainTexture);
    terrainModel->AddMesh(terrainMesh);

    subsurfaceMesh.AddTexture(terrainTexture);
    subsurfaceModel->AddMesh(subsurfaceMesh);
}

void CubeTerrain::GenerateSubsurfaceCubes(int offsetX, int offsetZ, int surfaceHeight, int bottom)
{
    
    int subCubesCount = abs(bottom - surfaceHeight) / 2;

    /*
    std::cout << "surface: " << surfaceHeight << std::endl;
    std::cout << "minDetph: " << minDepth << std::endl;
    std::cout << "cubesCount: " << subCubesCount << std::endl;
    std::cout << "offsetX: " << offsetX << std::endl;
    std::cout << "offsetZ: " << offsetZ << std::endl;
    */

    int addIndicesSize = additionalIndices.size();
    int addCubesIncrement = (addIndicesSize / 36) * 24;
    for (int i = 0; i < subCubesCount; i++) {
        int materialOffset = std::rand() % maxMaterialOffset;
        for (int k = 0; k < 24; k++) {
            Vertex vertex;
            vertex.position = cubePositions[k] + glm::vec3(offsetX * 2, bottom + (i * 2), offsetZ * 2);
            vertex.normal = cubeNormals[k / 4];
            vertex.texUV = cubeUVs[k % 4];
            vertex.meshCenterHeight = bottom + (i * 2);
            vertex.textureIndex = GetBlockMaterial(bottom + (i * 2), vertex.normal, materialOffset);
            additionalVertices.push_back(vertex);
            /*
            std::cout << vertex.position.x << " " << vertex.position.y << " " << vertex.position.z << std::endl;
            std::cout << vertex.normal.x << " " << vertex.normal.y << " " << vertex.normal.z << std::endl;
            std::cout << vertex.texUV.x << " " << vertex.texUV.y << std::endl;
            std::cout << "----" << std::endl;
            */
        }

        for (int j = 0; j < cubeIndices.size(); j++) {
            //additionalIndices.push_back(cubeIndices[j] + i * 24 + size * size * 24 + addIndicesSize);
            additionalIndices.push_back(cubeIndices[j] + i * 24 + addCubesIncrement);
        }
    }
}

void CubeTerrain::GenerateBottomCube(int offsetX, int offsetZ)
{
    int addIndicesSize = additionalIndices.size();
    int addCubesIncrement = (addIndicesSize / 36) * 24;

    for (int k = 0; k < 24; k++) {
        Vertex vertex;
        vertex.position = cubePositions[k] + glm::vec3(offsetX * 2, minDepth, offsetZ * 2);
        vertex.normal = cubeNormals[k / 4];
        vertex.texUV = cubeUVs[k % 4];
        additionalVertices.push_back(vertex);
    }

    for (int j = 0; j < cubeIndices.size(); j++) {
        additionalIndices.push_back(cubeIndices[j] + addCubesIncrement);
    }
}

void CubeTerrain::Draw(Shader& shader, Camera& camera) 
{
    shader.Bind();
    shader.SetUniform3f("u_GrassIndices", grass.x, grass.y, grass.z);
    terrainModel->Draw(shader, camera);
    subsurfaceModel->Draw(shader, camera);
}

int CubeTerrain::GetCubeHeight(int offsetX, int offsetZ) 
{
    if (flat) {
        return 0;
    }
    else if (heightMapped) {
        int centerX = offsetX + cubeCenter.x;
        int centerZ = offsetZ + cubeCenter.z;

        unsigned bytesPerPixel = 3;
        unsigned char* pixelOffset = terrainHeightMap + (centerX + mapHeight * centerZ) * bytesPerPixel;
        unsigned char r = pixelOffset[0];
        unsigned char g = pixelOffset[1];
        unsigned char b = pixelOffset[2];
        int rBit = r << 16;
        int gBit = g << 8;
        int bBit = b;

        float height = rBit + gBit + bBit;
        height += maxPixColour / (float)2;
        height /= maxPixColour / (float)2;

        int finalHeight = height * maxHeight;
        finalHeight -= finalHeight % 2;

        std::cout << finalHeight << std::endl;

        return finalHeight;
    }
    else if (procedural)
    {
        int height = (int)heightGenerator->GenerateHeight(offsetX, offsetZ);
        height -= height % 2;
        //std::cout << height << std::endl;
        return height - 1;
    }
    
}

int CubeTerrain::GetNeighbourMinHeight(int offsetX, int offsetZ, int height)
{
    int min = std::min({
        GetCubeHeight(offsetX + 1, offsetZ),
        GetCubeHeight(offsetX - 1, offsetZ),
        GetCubeHeight(offsetX, offsetZ + 1),
        GetCubeHeight(offsetX, offsetZ - 1)
    });

    return min;
}



int CubeTerrain::GetYCoord(int offsetX, int offsetZ) 
{
    float terrainX = offsetX - x;
    float terrainZ = offsetZ - z;

    int gridX = (int)(terrainX / 2);
    int gridZ = (int)(terrainZ / 2);

    return heights[gridX][gridZ];
}

int CubeTerrain::GetSize()
{
    return size;
}
