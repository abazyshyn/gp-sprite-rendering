#include "pch.h"

#include "sprite.h"

namespace GP
{

    CSprite::CSprite()
        : m_vertexBuffer(nullptr),
          m_indexBuffer(nullptr),
          m_textures(),
          m_currentTextureIndex(0),
          m_frameTime(0.0f),
          m_cycleTime(0.0f),
          m_vertexCount(0),
          m_indexCount(0),
          m_screenWidth(0),
          m_screenHeight(0),
          m_bitmapWidth(0),
          m_bitmapHeight(0),
          m_renderX(0),
          m_renderY(0),
          m_prevPosX(0),
          m_prevPosY(0)
    {
    }

    bool CSprite::Init(std::string spriteFilename, ID3D11Device *device, ID3D11DeviceContext *deviceContext,
                       int32_t screenWidth, int32_t screenHeight, int32_t rednerX, int32_t renderY)
    {
        m_screenWidth = screenWidth;
        m_screenHeight = screenHeight;

        m_renderX = rednerX;
        m_renderY = renderY;

        m_frameTime = 0.0f;

        if (!InitBuffers(device))
        {
            return false;
        }

        if (!LoadTextures(device, deviceContext, spriteFilename))
        {
            return false;
        }

        return true;
    }

    void CSprite::Shutdown()
    {
        ShutdownBuffers();
        ReleaseTextures();
    }

    void CSprite::SetRenderLocation(int32_t x, int32_t y)
    {
        m_renderX = x;
        m_renderY = y;
    }

    bool CSprite::InitBuffers(ID3D11Device *device)
    {
        m_vertexCount = 6;
        m_indexCount = m_vertexCount;

        std::vector<Vertex_s> vertices(static_cast<size_t>(m_vertexCount));
        std::vector<size_t> indices(static_cast<size_t>(m_indexCount));

        m_prevPosX = -1;
        m_prevPosY = -1;

        for (size_t i = 0; i < indices.size(); ++i)
        {
            indices[i] = i;
        }

        D3D11_BUFFER_DESC vertexBufferDescription{};
        vertexBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
        vertexBufferDescription.ByteWidth = static_cast<uint32_t>(vertices.size()) * static_cast<uint32_t>(sizeof(Vertex_s));
        vertexBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vertexBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        vertexBufferDescription.MiscFlags = 0;
        vertexBufferDescription.StructureByteStride = 0;

        D3D11_SUBRESOURCE_DATA vertexData{};
        vertexData.pSysMem = vertices.data();
        vertexData.SysMemPitch = 0;
        vertexData.SysMemSlicePitch = 0;

        if (FAILED(device->CreateBuffer(&vertexBufferDescription, &vertexData, &m_vertexBuffer)))
        {
            return false;
        }

        D3D11_BUFFER_DESC indexBufferDescription{};
        indexBufferDescription.Usage = D3D11_USAGE_DEFAULT;
        indexBufferDescription.ByteWidth = static_cast<uint32_t>(indices.size()) * static_cast<uint32_t>(sizeof(size_t));
        indexBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexBufferDescription.CPUAccessFlags = 0;
        vertexBufferDescription.MiscFlags = 0;
        vertexBufferDescription.StructureByteStride = 0;

        D3D11_SUBRESOURCE_DATA indexData{};
        indexData.pSysMem = indices.data();
        indexData.SysMemPitch = 0;
        indexData.SysMemSlicePitch = 0;

        if (FAILED(device->CreateBuffer(&indexBufferDescription, &indexData, &m_indexBuffer)))
        {
            return false;
        }

        return true;
    }

    void CSprite::ShutdownBuffers()
    {
        if (m_indexBuffer)
        {
            m_indexBuffer->Release();
            m_indexBuffer = 0;
        }

        if (m_vertexBuffer)
        {
            m_vertexBuffer->Release();
            m_vertexBuffer = 0;
        }
    }

    bool CSprite::LoadTextures(ID3D11Device *device, ID3D11DeviceContext *deviceContext, std::string filename)
    {
        std::ifstream fin;
        std::filesystem::path texturePath{std::filesystem::current_path() / "res" / "models" / filename};
        fin.open(texturePath);
        if (fin.fail())
        {
            return false;
        }

        size_t textureCount = 0;
        fin >> textureCount;

        m_textures.resize(textureCount);

        char input;
        fin.get(input);

        std::string textureFilename{};
        for (size_t i = 0; i < m_textures.size(); ++i)
        {
            fin.get(input);
            while (input != '\0')
            {
                textureFilename.push_back(input);
                fin.get(input);
            }

            if (!m_textures[i].Init(device, deviceContext, textureFilename))
            {
                return false;
            }

            textureFilename.clear();
        }

        fin >> m_cycleTime;
        m_cycleTime *= 0.001f; // Convert the integer milliseconds to float representation.

        fin.close();

        m_bitmapWidth = m_textures[0].GetWidth();
        m_bitmapHeight = m_textures[0].GetHeight();
        m_currentTextureIndex = 0;

        return true;
    }

    void CSprite::ReleaseTextures()
    {
        for (CTexture &texture : m_textures)
        {
            texture.Shutdown();
        }
    }

} // namespace GP
