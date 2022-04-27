#include "TestCubeTerrain.h"
#include <GL/glew.h>

#include "imgui.h"
namespace test_model
{
    TestCubeTerrain::TestCubeTerrain() 
        : m_ClearColor {0.2f, 0.3f, 0.8f, 1.0f}
    {
    }
    
    TestCubeTerrain::TestCubeTerrain(GLFWwindow* window, int width, int height) 
        : m_ClearColor {0.2f, 0.3f, 0.8f, 1.0f}, WIDTH(width), HEIGHT(height), m_Window(window)
    {
        glEnable(GL_DEPTH_TEST);

        // Models
        //m_Cube = std::make_unique<Model>("../res/models/box/box.obj");
        //Texture dirtTexture("../res/textures/block_game/blocks.png", 1);
        //m_Terrain = std::make_unique<CubeTerrain>(0, 0, "../res/textures/block_game/dirt_texture_highres.jpg", "../res/textures/heightmap.png");
        m_Terrain = std::make_unique<CubeTerrain>(0, 0, 200,"../res/textures/block_game/jdh_blocks_atlas.png", true);

        glm::vec3 camPos = glm::vec3(m_Terrain->GetSize(), m_Terrain->GetYCoord(m_Terrain->GetSize(), m_Terrain->GetSize()) + 4, m_Terrain->GetSize());
        m_Camera = std::make_unique<Camera>(WIDTH, HEIGHT, 
                                            camPos,
                                            glm::vec3(-0.482962, -0.19509, -0.853632),
                                            glm::vec3(0, 1, 0)
                                            );
        
        // Shaders
        m_SimpleShader = std::make_unique<Shader>("../res/shaders/cubeTerrain.shader");
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
    
    TestCubeTerrain::~TestCubeTerrain() 
    {
    }
    
    
    void TestCubeTerrain::OnUpdate(float deltaTime) 
    {
        
    }
    
    void TestCubeTerrain::OnRender() 
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

        //m_LightShader->Bind();
        //m_Camera->Matrix(*m_LightShader, "u_CameraMatrix");

        //m_LightCube->Draw(*m_LightShader, *m_Camera, glm::vec3(1.0, 1.0, 1.0), lightPos);
        //m_Terrain->terrainModel->Draw(*m_SimpleShader, *m_Camera);
        //m_LightCube->Draw(*m_SimpleShader, *m_Camera, glm::vec3(1.0, 1.0, 1.0), lightPos);

    }
    
    void TestCubeTerrain::OnImGuiRender() 
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