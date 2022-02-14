#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) 
    : m_Vertices(vertices), m_Indices(indices)
{
    m_VAO = new VertexArray();
    m_VAO->Bind();
    m_IndexBuffer = new IndexBuffer(m_Indices);
    m_VertexBuffer = new VertexBuffer(m_Vertices);

    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(2);
    layout.Push<float>(3);
    m_VAO->AddBuffer(*m_VertexBuffer, layout);
    

}

Mesh::Mesh(std::vector<Vertex>& vertices) 
    : m_Vertices(vertices)
{
    m_VAO = new VertexArray();
    m_VAO->Bind();
    m_VertexBuffer = new VertexBuffer(m_Vertices);

    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(2);
    layout.Push<float>(3);
    m_VAO->AddBuffer(*m_VertexBuffer, layout);
    

}

Mesh::~Mesh() 
{
    //delete m_VAO;
    //delete m_IndexBuffer;
    //delete m_VertexBuffer;
    
}

void Mesh::AddTexture(Texture& newTexture) 
{
    m_Textures.push_back(newTexture);
    HasTextures = true;
}

void Mesh::Draw(Shader& shader, Camera& camera, glm::vec3 scale, glm::vec3 translate) 
{
    
    shader.Bind();
    m_VAO->Bind();
    unsigned int numDiffuse = 0;
    unsigned int numSpecular = 0;

    Renderer renderer;

    for (unsigned int i = 0; i < m_Textures.size(); i++){
        m_Textures[i].SetUnit(shader, "u_Texture", 1);
        m_Textures[i].Bind();
        //std::cout << "We have texture" <<std::endl;
    }

    if (HasColors && !HasTextures)
    {
        shader.SetUniform3f("u_AmbientColor", ambient.r, ambient.g, ambient.b);
        shader.SetUniform3f("u_DiffuseColor", diffuse.r, diffuse.g, diffuse.b);
        shader.SetUniform3f("u_SpecularColor", specular.r, specular.g, specular.b);
    }

    /*
    for (Vertex vertex : m_Vertices) {
        std::cout<<vertex.position.x << " " <<vertex.position.z<< std::endl;
    }

    for (unsigned int index : m_Indices)
        std::cout << index << std::endl;
    */

    //shader.SetUniform1f("u_Opacity", opacity);
    shader.SetUniform1i("u_HasTexture", int(HasTextures));

    //shader.SetUniform3f("u_CameraPosition", camPos.x, camPos.y, camPos.z);
    //camera.Matrix(shader, "u_CameraMatrix");
    //glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::mat4 objectModelInit = glm::mat4(1.0f);
    objectModelInit = glm::translate(objectModelInit, translate);
    objectModelInit = glm::scale(objectModelInit, scale);

    shader.SetUniformMat4f("u_ModelMatrix", model_matrix);
    shader.SetUniformMat4f("u_Transform", objectModelInit);

    renderer.Draw(*m_VAO, *m_IndexBuffer, shader);

    // NEEDS TO BE TESTED WHEN MULTIPLE MESHES ARE PRESENT
    for (unsigned int i = 0; i < m_Textures.size(); i++){
        m_Textures[i].Unbind();
    }
    

}

void Mesh::SetColors(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) 
{
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;

    this->HasColors = true;
}

void Mesh::SetOpacity(float opacity)
{
    this->opacity = opacity;
}

void Mesh::SetModelMatrix(glm::mat4 model_matrix)
{
    this->model_matrix = model_matrix;
}
