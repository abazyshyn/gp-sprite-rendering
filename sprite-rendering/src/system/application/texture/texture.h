#pragma once

namespace GP
{

    class CTexture
    {
    public:
        CTexture();

        bool Init(ID3D11Device *device, ID3D11DeviceContext *deviceContext, std::string filename);
        void Shutdown();

        int32_t GetWidth() const { return m_width; }
        int32_t GetHeight() const { return m_height; }
        ID3D11ShaderResourceView *GetTexture() const { return m_textureView; }

    private:
        bool LoadTexture(std::string filename);

    private:
        uint_least8_t *m_textureData;
        ID3D11Texture2D *m_texture;
        ID3D11ShaderResourceView *m_textureView; // Shader uses to access the texture data when drawing
        int32_t m_width;
        int32_t m_height;
    };

} // namespace GP
