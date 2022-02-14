cmake_minimum_required ( VERSION 3.1.0 )

project ( TerrainGeneration
  VERSION 3.0.0
  LANGUAGES CXX
)

# Set to c++14
set ( CMAKE_CXX_STANDARD 14 )

# Source
set ( SOURCE
  src/vendor/glm/glm.hpp
  src/vendor/glm/gtx/string_cast.hpp
  src/vendor/imgui/libs/imgui/imgui.h
  src/vendor/imgui/libs/imgui/imgui.cpp
  src/vendor/imgui/libs/imgui/backends/imgui_impl_glfw.h
  src/vendor/imgui/libs/imgui/backends/imgui_impl_glfw.cpp
  src/vendor/imgui/libs/imgui/backends/imgui_impl_opengl3.h
  src/vendor/imgui/libs/imgui/backends/imgui_impl_opengl3.cpp
  src/vendor/stb_image/stb_image.h
  src/vendor/stb_image/stb_image.cpp
  src/models/TestModel.cpp
  src/models/TestModel.h
  src/models/TestTerrain.cpp
  src/models/TestTerrain.h
  src/Application.cpp
  src/Camera.cpp
  src/Camera.h
  src/CubeMap.cpp
  src/CubeMap.h
  src/IndexBuffer.cpp
  src/IndexBuffer.h
  src/Mesh.cpp
  src/Mesh.h
  src/Model.cpp
  src/Model.h
  src/ProceduralGeneration.cpp
  src/ProceduralGeneration.h
  src/Renderer.cpp
  src/Renderer.h
  src/Shader.cpp
  src/Shader.h
  src/Terrain.cpp
  src/Terrain.h
  src/Texture.cpp
  src/Texture.h
  src/VertexArray.cpp
  src/VertexArray.h
  src/VertexBuffer.cpp
  src/VertexBuffer.h
  src/VertexBufferLayout.cpp
  src/VertexBufferLayout.h
  
)


# Executable
add_executable(TerrainGeneration ${SOURCE})

# GLFW linking
find_package(glfw3 3.3 REQUIRED)
target_link_libraries(TerrainGeneration glfw)
find_package(OpenGL REQUIRED)
target_link_libraries(TerrainGeneration OpenGL::GL)

# IMGUI linking
add_subdirectory("src/vendor/imgui")
target_link_libraries(TerrainGeneration IMGUI)

# GLEW linking
find_package(GLEW 2.0 REQUIRED)
target_link_libraries(TerrainGeneration GLEW::GLEW)

# ASSIMP linking
find_package(assimp REQUIRED)
target_link_libraries(TerrainGeneration assimp)

include_directories(src/vendor/imgui/libs/imgui/)