#include "TestTerrain.h"
#include <GL/glew.h>

#include "imgui.h"
namespace test_model
{
    TestTerrain::TestTerrain() 
        : m_ClearColor {0.2f, 0.3f, 0.8f, 1.0f}
    {
    }
    
    TestTerrain::TestTerrain(GLFWwindow* window, int width, int height) 
        : m_ClearColor {0.2f, 0.3f, 0.8f, 1.0f}, WIDTH(width), HEIGHT(height), m_Window(window)
    {
        glEnable(GL_DEPTH_TEST);

        // Models
        // m_Terrain = std::make_unique<Terrain>(0, 0, 800, 128, "../res/textures/grass.png");
        //m_Terrain = std::make_unique<Terrain>(0, 0, 800, "../res/textures/grass.png", "../res/textures/heightmap.png");
        m_Terrain = std::make_unique<Terrain>(0, 0, 800, 128, "../res/textures/grass.png", true);
        m_LightCube = std::make_unique<Model>("../res/models/lightCube/lightCube.obj");
        //m_Cube = std::make_unique<Model>("../res/models/lightCube/lightCube.obj");

        glm::vec3 camPos = glm::vec3(400, m_Terrain->GetYCoord(400, 400) + 5, 400);
        m_Camera = std::make_unique<Camera>(WIDTH, HEIGHT, 
                                            camPos,
                                            glm::vec3(-0.482962, -0.19509, -0.853632),
                                            glm::vec3(0, 1, 0)
                                            );
        
        // Shaders
        m_SimpleShader = std::make_unique<Shader>("../res/shaders/terrain.shader");
        m_LightShader = std::make_unique<Shader>("../res/shaders/lightCube.shader");
        m_SkyboxShader = std::make_unique<Shader>("../res/shaders/skybox.shader");
        //

        std::vector<std::string> faces
        {
            "../res/textures/skyboxes/thinmatrix_right.jpg",    // posx
            "../res/textures/skyboxes/thinmatrix_left.jpg",      // negx
            "../res/textures/skyboxes/thinmatrix_top.jpg",      // posy
            "../res/textures/skyboxes/thinmatrix_bottom.jpg",   // negy
            "../res/textures/skyboxes/thinmatrix_back.jpg",     // posz
            "../res/textures/skyboxes/thinmatrix_front.jpg"    // negz
        };



        m_SkyboxMap = std::make_unique<CubeMap>(faces);
    }
    
    TestTerrain::~TestTerrain() 
    {
    }
    
    
    void TestTerrain::OnUpdate(float deltaTime) 
    {
        
    }
    
    void TestTerrain::OnRender() 
    {
        glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_Camera->Input(m_Window);
        m_Camera->updateMatrix(45.0f, 0.1f, 1000.0f);

        glm::mat4 view = m_Camera->GetView();
        glm::vec3 viewPos = m_Camera->GetPosition();
        glm::mat4 projection = m_Camera->GetProjection();
        glm::mat4 camMetrix = m_Camera->GetMatrix();


        m_SkyboxShader->Bind();
        m_SkyboxShader->SetUniformMat4f("u_View", glm::mat4(glm::mat3(view)));
        m_SkyboxShader->SetUniformMat4f("u_Projection", projection);
        m_SkyboxShader->SetUniformMat4f("u_CameraMatrix", camMetrix);
        m_SkyboxMap->Draw(*m_SkyboxShader, *m_Camera);        


        m_SimpleShader->Bind();
        m_SimpleShader->SetUniform1f("u_LightStrength", lightStrength);
        m_SimpleShader->SetUniform3f("u_LightColor", lightColor.r, lightColor.g, lightColor.b);
        m_SimpleShader->SetUniform3f("u_ViewPos", viewPos.r, viewPos.g, viewPos.b);
        m_SimpleShader->SetUniform3f("u_LightPos", lightPos.x, lightPos.y, lightPos.z);
        m_Camera->Matrix(*m_SimpleShader, "u_CameraMatrix");
        m_Terrain->Draw(*m_SimpleShader, *m_Camera);
        //m_Cube->Draw(*m_SimpleShader, *m_Camera);

        m_LightShader->Bind();
        m_Camera->Matrix(*m_LightShader, "u_CameraMatrix");

        //m_LightCube->Draw(*m_LightShader, *m_Camera, glm::vec3(1.0, 1.0, 1.0), lightPos);
        //m_Terrain->terrainModel->Draw(*m_SimpleShader, *m_Camera);
        //m_LightCube->Draw(*m_SimpleShader, *m_Camera, glm::vec3(1.0, 1.0, 1.0), lightPos);

    }
    
    void TestTerrain::OnImGuiRender() 
    {
        ImGui::Begin("Test");
        ImGui::ColorEdit4("Clear Color", m_ClearColor);
        ImGui::End();

        if (ImGui::CollapsingHeader("Light")){
            ImGui::Indent();
		    ImGui::SliderFloat("Intensity", &lightStrength, 0, 20);
            ImGui::SliderFloat3("Light Position", &lightPos.x, 0, 800);
            ImGui::Unindent();
        }
    }
}