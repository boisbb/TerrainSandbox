#pragma once

#include <string>
#include <vector>
#include <memory>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Camera.h"
#include "Texture.h"
#include "Renderer.h"

#include "stb_image/stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class CubeMap
{
public:
    CubeMap(vector<std::string> faces);
    ~CubeMap();
    void Draw(Shader& shader, Camera& camera);

    inline unsigned int GetTextureID() const { return skyboxTexture; }

private:
    unsigned int loadCubemap(vector<std::string> faces);
    
    unsigned int skyboxVBO, skyboxVAO, skyboxTexture;

    VertexArray* m_VAO;
    IndexBuffer* m_IndexBuffer;
    VertexBuffer* m_VertexBuffer;

};