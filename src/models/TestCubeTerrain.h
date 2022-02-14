#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "TestModel.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "Camera.h"
#include "Model.h"
#include "CubeMap.h"
#include "CubeTerrain.h"

namespace test_model {
    class TestCubeTerrain : public TestModel{
    public:
        TestCubeTerrain();
        TestCubeTerrain(GLFWwindow* window, int width, int height);
        ~TestCubeTerrain();

        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;
    private: 
        float m_ClearColor[4];
        GLFWwindow* m_Window;

        std::unique_ptr<CubeTerrain> m_Terrain;
        std::unique_ptr<Model> m_LightCube;
        std::unique_ptr<Model> m_Cube;

        std::unique_ptr<CubeMap> m_SkyboxMap;

        std::unique_ptr<Camera> m_Camera;
        std::unique_ptr<Shader> m_SimpleShader;
        std::unique_ptr<Shader> m_LightShader;
        std::unique_ptr<Shader> m_SkyboxShader;

        glm::vec3 lightPos = glm::vec3(0.0f,10000.0f,-7000.0f);
        glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
        float lightStrength = 0.5f;

        int WIDTH;
        int HEIGHT;
    };
}