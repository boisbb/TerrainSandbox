#pragma once

#include <array>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vendor/glm/glm.hpp"


struct Vertex
{
  glm::vec3 position;
  glm::vec2 texUV;
  glm::vec3 normal;
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


