#include "pch.h"

#include "sprite.h"

namespace GP
{

    CSprite::CSprite()
        : m_vertexBuffer(nullptr),
          m_indexBuffer(nullptr),
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
                       int32_t screenWidth, int32_t screenHeight, int32_t rednerX, int32_t renderY,
                       int32_t renderWidth, int32_t renderHeight)
    {
        m_screenWidth = screenWidth;
        m_screenHeight = screenHeight;

        m_renderX = rednerX;
        m_renderY = renderY;
        m_renderWidth = renderWidth;
        m_renderHeight = renderHeight;

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

    bool CSprite::Render(ID3D11DeviceContext *deviceContext)
    {
        if (!UpdateBuffers(deviceContext))
        {
            return false;
        }

        RenderBuffers(deviceContext);

        return true;
    }

    void CSprite::Update(float frameTime)
    {
        m_frameTime += frameTime;

        if (m_frameTime >= m_cycleTime)
        {
            m_frameTime -= m_cycleTime;
            m_currentTextureIndex += 1;
            if (m_currentTextureIndex == m_textures.size())
            {
                m_currentTextureIndex = 0;
            }
        }
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
        std::vector<uint32_t> indices(static_cast<size_t>(m_indexCount));

        m_prevPosX = -1;
        m_prevPosY = -1;

        for (size_t i = 0; i < indices.size(); ++i)
        {
            indices[i] = static_cast<uint32_t>(i);
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
        indexBufferDescription.ByteWidth = static_cast<uint32_t>(indices.size()) * static_cast<uint32_t>(sizeof(uint32_t));
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
            m_indexBuffer = nullptr;
        }

        if (m_vertexBuffer)
        {
            m_vertexBuffer->Release();
            m_vertexBuffer = nullptr;
        }
    }

    bool CSprite::UpdateBuffers(ID3D11DeviceContext *deviceContext)
    {
        if ((m_prevPosX == m_renderX) && (m_prevPosY == m_renderY))
        {
            return true;
        }

        m_prevPosX = m_renderX;
        m_prevPosY = m_renderY;

        float left = (static_cast<float>(m_screenWidth) / 2.0f * -1.0f) + static_cast<float>(m_renderX);
        float right = left + static_cast<float>(m_renderWidth ? m_renderWidth : m_bitmapWidth);
        float top = static_cast<float>(m_screenHeight) / 2.0f - static_cast<float>(m_renderY);
        float bottom = top - static_cast<float>(m_renderHeight ? m_renderHeight : m_bitmapHeight);

        std::vector<Vertex_s> vertices(m_vertexCount);

        vertices[0].position = XMFLOAT3(left, top, 0.0f);
        vertices[0].texture = XMFLOAT2(0.0f, 0.0f);
        vertices[1].position = XMFLOAT3(right, bottom, 0.0f);
        vertices[1].texture = XMFLOAT2(1.0f, 1.0f);
        vertices[2].position = XMFLOAT3(left, bottom, 0.0f);
        vertices[2].texture = XMFLOAT2(0.0f, 1.0f);
        vertices[3].position = XMFLOAT3(left, top, 0.0f);
        vertices[3].texture = XMFLOAT2(0.0f, 0.0f);
        vertices[4].position = XMFLOAT3(right, top, 0.0f);
        vertices[4].texture = XMFLOAT2(1.0f, 0.0f);
        vertices[5].position = XMFLOAT3(right, bottom, 0.0f);
        vertices[5].texture = XMFLOAT2(1.0f, 1.0f);

        D3D11_MAPPED_SUBRESOURCE mappedResource{};
        if (FAILED(deviceContext->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
        {
            return false;
        }

        Vertex_s *vertexBufferData = static_cast<Vertex_s *>(mappedResource.pData);
        memcpy(vertexBufferData, vertices.data(), vertices.size() * sizeof(Vertex_s));

        deviceContext->Unmap(m_vertexBuffer, 0);
        vertexBufferData = nullptr;

        return true;
    }

    void CSprite::RenderBuffers(ID3D11DeviceContext *deviceContext)
    {
        const uint32_t vertexBufferStride = static_cast<uint32_t>(sizeof(Vertex_s));
        const uint32_t vertexBufferOffset = 0;
        const uint32_t indexBufferOffset = 0;

        deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &vertexBufferStride, &vertexBufferOffset);
        deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, indexBufferOffset);
        deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }

    bool CSprite::LoadTextures(ID3D11Device *device, ID3D11DeviceContext *deviceContext, std::string filename)
    {
        std::ifstream fin;
        std::filesystem::path texturePath{std::filesystem::current_path() / "res" / "sprites" / filename};
        fin.open(texturePath);
        if (fin.fail())
        {
            return false;
        }

        size_t textureCount = 0;
        fin >> textureCount;

        m_textures.resize(textureCount);

        char input{};
        fin.get(input);

        std::string textureFilename{};
        for (CTexture &texture : m_textures)
        {
            fin.get(input);
            while (input != '\n')
            {
                textureFilename.push_back(input);
                fin.get(input);
            }

            if (!texture.Init(device, deviceContext, textureFilename))
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
