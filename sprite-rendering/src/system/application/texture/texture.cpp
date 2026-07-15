#include "pch.h"

#include "texture.h"
#include "stb_image/stb_image.h"

namespace GP
{

    CTexture::CTexture()
        : m_textureData(nullptr),
          m_texture(nullptr),
          m_textureView(nullptr),
          m_width(0),
          m_height(0)
    {
    }

    bool CTexture::Init(ID3D11Device *device, ID3D11DeviceContext *deviceContext, std::string filename)
    {
        if (!LoadTexture(filename))
        {
            return false;
        }

        D3D11_TEXTURE2D_DESC textureDescription{};
        textureDescription.Width = m_width;
        textureDescription.Height = m_height;
        textureDescription.MipLevels = 0;
        textureDescription.ArraySize = 0;
        textureDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        textureDescription.SampleDesc.Count = 1;
        textureDescription.SampleDesc.Quality = 0;
        textureDescription.Usage = D3D11_USAGE_DEFAULT;
        textureDescription.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
        textureDescription.CPUAccessFlags = 0;
        textureDescription.MiscFlags = 0;

        if (FAILED(device->CreateTexture2D(&textureDescription, nullptr, &m_texture)))
        {
            return false;
        }

        // Copy the texture data into the texture
        uint32_t rowPitch = static_cast<uint32_t>(m_width * 4) * static_cast<uint32_t>(sizeof(uint_least8_t));
        deviceContext->UpdateSubresource(m_texture, 0, nullptr, m_textureData, rowPitch, 0);

        D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDescription{};
        shaderResourceViewDescription.Format = textureDescription.Format;
        shaderResourceViewDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        shaderResourceViewDescription.Texture2D.MostDetailedMip = 0;
        shaderResourceViewDescription.Texture2D.MipLevels = -1;

        if (FAILED(device->CreateShaderResourceView(m_texture, &shaderResourceViewDescription, &m_textureView)))
        {
            return false;
        }

        deviceContext->GenerateMips(m_textureView);

        stbi_image_free(m_textureData);
        m_textureData = nullptr;

        return true;
    }

    void CTexture::Shutdown()
    {
        if (m_textureView)
        {
            m_textureView->Release();
            m_textureView = nullptr;
        }

        if (m_texture)
        {
            m_texture->Release();
            m_texture = nullptr;
        }

        if (m_textureData)
        {
            stbi_image_free(m_textureData);
            m_textureData = nullptr;
        }
    }

    bool CTexture::LoadTexture(std::string filename)
    {
        std::filesystem::path texturePath{std::filesystem::current_path() / "res" / "textures" / filename};

        // stbi_set_flip_vertically_on_load(true);
        int32_t channels = 0;
        m_textureData = stbi_load(texturePath.string().c_str(), &m_width, &m_height, &channels, 0);

        if (!m_textureData)
        {
            stbi_image_free(m_textureData);
            return false;
        }

        return true;
    }

} // namespace GP
