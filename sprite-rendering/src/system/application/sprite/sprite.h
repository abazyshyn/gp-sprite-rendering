#pragma once

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "system/application/texture/texture.h"

namespace GP
{

    class CSprite
    {
    public:
        CSprite();

        bool Init(std::string spriteFilename, ID3D11Device *device, ID3D11DeviceContext *deviceContext,
                  int32_t screenWidth, int32_t screenHeight, int32_t rednerX, int32_t renderY);
        void Shutdown();
        bool Render(ID3D11DeviceContext *deviceContext);
        void Update(float frameTime);

        int32_t GetIndexCount() const { return m_indexCount; }
        ID3D11ShaderResourceView *GetTexture() { return m_textures[m_currentTextureIndex].GetTexture(); }

        void SetRenderLocation(int32_t x, int32_t y);

    private:
        bool InitBuffers(ID3D11Device *device);
        void ShutdownBuffers();
        bool UpdateBuffers(ID3D11DeviceContext *deviceContext);
        void RenderBuffers(ID3D11DeviceContext *deviceContext);

        bool LoadTextures(ID3D11Device *device, ID3D11DeviceContext *deviceContext, std::string filename);
        void ReleaseTextures();

    private:
        struct Vertex_s
        {
            XMFLOAT3 position;
            XMFLOAT2 texture;
        };

        std::vector<CTexture> m_textures;
        ID3D11Buffer *m_vertexBuffer;
        ID3D11Buffer *m_indexBuffer;
        size_t m_currentTextureIndex;
        float m_frameTime;
        float m_cycleTime;
        int32_t m_vertexCount;
        int32_t m_indexCount;
        int32_t m_screenWidth;
        int32_t m_screenHeight;
        int32_t m_bitmapWidth;
        int32_t m_bitmapHeight;
        int32_t m_renderX;
        int32_t m_renderY;
        int32_t m_prevPosX;
        int32_t m_prevPosY;
    };

} // namespace GP
