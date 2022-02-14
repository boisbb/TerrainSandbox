#pragma once

#include <string>
#include <vector>
#include <memory>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Camera.h"
#include "Texture.h"
#include "Renderer.h"

class Mesh
{
public:
    Mesh() {}
    Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices);
    Mesh(std::vector<Vertex> &vertices);
    ~Mesh();

    void AddTexture(Texture &newTexture);

    void Draw(Shader &shader, Camera &camera, glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 translate = glm::vec3(0.0f, 0.0f, 0.0f));

    bool HasTexture() const { return m_Textures.size() == 0; }

    glm::vec3 GetAmbient() const { return ambient; }
    glm::vec3 GetDiffuse() const { return diffuse; }
    glm::vec3 GetSpecular() const { return specular; }
    float GetOpacity() const { return opacity; }
    glm::mat4 GetModelMatrix() const { return model_matrix; }

    bool HasColor() const { return HasColors; }

    void SetColors(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
    void SetOpacity(float opacity);
    void SetModelMatrix(glm::mat4 model_matrix);

private:
    std::vector<Vertex> m_Vertices;
    std::vector<unsigned int> m_Indices;
    std::vector<Texture> m_Textures;

    bool HasColors = false;
    bool HasTextures = false;

    glm::vec3 diffuse;
    glm::vec3 ambient;
    glm::vec3 specular;
    float opacity = 1.0;

    glm::mat4 model_matrix = glm::mat4(1.0f);

    unsigned int VBO, EBO, VAO;

    VertexArray *m_VAO;
    IndexBuffer *m_IndexBuffer;
    VertexBuffer *m_VertexBuffer;
};