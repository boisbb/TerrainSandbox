#pragma once

#include <array>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"


struct Vertex
{
  glm::vec3 position;
  glm::vec2 texUV;
  glm::vec3 normal;
  // By default when not used, set to 0
  // thats a very stupid workaround TODO
  float meshCenterHeight = 0.0f;
  float textureIndex = 0.0f;
};

class VertexBuffer{
private:
  unsigned int m_RendererID;
public:
  VertexBuffer(std::vector<Vertex>& vertices);
  VertexBuffer(std::vector<glm::vec3>& vertices);
  ~VertexBuffer();

  void Bind() const;
  void Unbind() const;
};


