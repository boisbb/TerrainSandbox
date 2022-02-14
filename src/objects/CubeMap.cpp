#include "CubeMap.h"

/*
    Some of the code was for Cube map was influenced by code derived from this tutorial:
    https://learnopengl.com/Advanced-OpenGL/Cubemaps
    https://github.com/JoeyDeVries/LearnOpenGL/tree/master/src/4.advanced_opengl/6.2.cubemaps_environment_mapping
    Author: Joe de Vriez (https://twitter.com/JoeyDeVriez)
    licensed under CC BY 4.0
*/

/**
 * @brief Construct a new Cube Map:: Cube Map object.
 * 
 * @param faces Vector of paths to Skybox faces - X/Y/Z, pos/neg
 */
CubeMap::CubeMap(std::vector<std::string> faces){
    std::vector<glm::vec3> skyboxVertices = {
        // positions          
        glm::vec3{-1.0f,  1.0f, -1.0f},
        glm::vec3{-1.0f, -1.0f, -1.0f},
        glm::vec3{ 1.0f, -1.0f, -1.0f},
        glm::vec3{ 1.0f, -1.0f, -1.0f},
        glm::vec3{ 1.0f,  1.0f, -1.0f},
        glm::vec3{-1.0f,  1.0f, -1.0f},

        glm::vec3{-1.0f, -1.0f,  1.0f},
        glm::vec3{-1.0f, -1.0f, -1.0f},
        glm::vec3{-1.0f,  1.0f, -1.0f},
        glm::vec3{-1.0f,  1.0f, -1.0f},
        glm::vec3{-1.0f,  1.0f,  1.0f},
        glm::vec3{-1.0f, -1.0f,  1.0f},

        glm::vec3{ 1.0f, -1.0f, -1.0f},
        glm::vec3{ 1.0f, -1.0f,  1.0f},
        glm::vec3{ 1.0f,  1.0f,  1.0f},
        glm::vec3{ 1.0f,  1.0f,  1.0f},
        glm::vec3{ 1.0f,  1.0f, -1.0f},
        glm::vec3{ 1.0f, -1.0f, -1.0f},

        glm::vec3{-1.0f, -1.0f,  1.0f},
        glm::vec3{-1.0f,  1.0f,  1.0f},
        glm::vec3{ 1.0f,  1.0f,  1.0f},
        glm::vec3{ 1.0f,  1.0f,  1.0f},
        glm::vec3{ 1.0f, -1.0f,  1.0f},
        glm::vec3{-1.0f, -1.0f,  1.0f},

        glm::vec3{-1.0f,  1.0f, -1.0f},
        glm::vec3{ 1.0f,  1.0f, -1.0f},
        glm::vec3{ 1.0f,  1.0f,  1.0f},
        glm::vec3{ 1.0f,  1.0f,  1.0f},
        glm::vec3{-1.0f,  1.0f,  1.0f},
        glm::vec3{-1.0f,  1.0f, -1.0f},

        glm::vec3{-1.0f, -1.0f, -1.0f},
        glm::vec3{-1.0f, -1.0f,  1.0f},
        glm::vec3{ 1.0f, -1.0f, -1.0f},
        glm::vec3{ 1.0f, -1.0f, -1.0f},
        glm::vec3{-1.0f, -1.0f,  1.0f},
        glm::vec3{ 1.0f, -1.0f,  1.0f}
    };

    m_VAO = new VertexArray();
    m_VAO->Bind();
    m_VertexBuffer = new VertexBuffer(skyboxVertices);

    VertexBufferLayout layout;
    layout.Push<float>(3);
    m_VAO->AddBuffer(*m_VertexBuffer, layout);

    skyboxTexture = loadCubemap(faces);

}

/**
 * @brief Draw the Skybox.
 * 
 * @param shader 
 * @param camera 
 */
void CubeMap::Draw(Shader& shader, Camera& camera){
    glDepthFunc(GL_LEQUAL);
    shader.Bind();

    glm::mat4 objectModelInit = glm::mat4(1.0f);
    objectModelInit = glm::scale(objectModelInit, glm::vec3(500,500,500));

    shader.SetUniformMat4f("u_Scale", objectModelInit);
    // skybox cube
    m_VAO->Bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    m_VAO->Unbind();

    glDepthFunc(GL_LESS);
}

CubeMap::~CubeMap(){

}


/**
 * @brief Create the cubemap texture.
 * 
 * @param faces 
 * @return unsigned int
 */
unsigned int CubeMap::loadCubemap(vector<std::string> faces){
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    stbi_set_flip_vertically_on_load(0);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Incorrect Cubemap texture path." << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}