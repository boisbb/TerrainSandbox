#pragma once

#include "Renderer.h"

class Texture{
private:
    unsigned int m_RendererID;
    std::string m_FilePath;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_BPP;
    unsigned int unit = 0;
    const char* m_Type;
public:
    Texture() {};
    Texture(const std::string& path, unsigned int slot);
    
    ~Texture();
    
    void SetUnit(Shader& shader, std::string uniform, unsigned int unit);
    void Bind();
    void Unbind() const;

    inline int GetWidth() const{ return m_Width; }
    inline int GetHeight() const{ return m_Height; }
    inline const char* GetType() const{ return m_Type; }
    inline std::string GetPath() const {return m_FilePath; }
    inline const unsigned int GetID() const { return m_RendererID; }
};