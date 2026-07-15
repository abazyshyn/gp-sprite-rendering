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

        int32_t GetIndexCount() const { return 0; }
        ID3D11ShaderResourceView *GetTexture() { return nullptr; }

        void SetRenderLocation(int32_t x, int32_t y);

    private:
        bool InitBuffers(ID3D11Device *device);
        void ShutdownBuffers();
        bool UpdateBuffers(ID3D11DeviceContext *deviceContext);
        void RenderBuffers(ID3D11DeviceContext *deviceContext);

        bool LoadTextures(ID3D11Device *device, ID3D11DeviceContext *deviceContext, std::string filename);
        void ReleaseTexture();

    private:
        struct Vertex_s
        {
            XMFLOAT3 position;
            XMFLOAT2 texture;
        };

        ID3D11Buffer *m_vertexBuffer;
        ID3D11Buffer *m_indexBuffer;
        std::vector<CTexture> m_textures;
        unsigned long m_currentTextureIndex;
        float m_frameTime;
        float m_cycleTime;
        int m_vertexCount;
        int m_indexCount;
        int m_screenWidth;
        int m_screenHeight;
        int m_bitmapWidth;
        int m_bitmapHeight;
        int m_renderX;
        int m_renderY;
        int m_prevPosX;
        int m_prevPosY;

        unsigned char _pad[4] = {0}; // Padding
    };

} // namespace GP
