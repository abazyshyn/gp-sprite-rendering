#pragma once

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
    };

} // namespace GP
