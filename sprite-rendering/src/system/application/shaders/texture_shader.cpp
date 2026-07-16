#include "pch.h"

#include "texture_shader.h"

namespace GP
{

    CTextureShader::CTextureShader()
        : m_vertexShader(nullptr),
          m_pixelShader(nullptr),
          m_inputLayout(nullptr),
          m_matrixBuffer(nullptr),
          m_sampleState(nullptr)
    {
    }

    bool CTextureShader::Init(ID3D11Device *device, HWND hWnd)
    {
        std::filesystem::path vsFilename{std::filesystem::current_path() / "res/shaders/texture_vs.hlsl"};
        std::filesystem::path psFilename{std::filesystem::current_path() / "res/shaders/texture_ps.hlsl"};

        if (!InitShader(device, hWnd, vsFilename, psFilename))
        {
            return false;
        }

        return true;
    }

    bool CTextureShader::InitShader(ID3D11Device *device, HWND hWnd, const std::filesystem::path &vsFilename, const std::filesystem::path &psFilename)
    {
        ID3DBlob *errorMessage = nullptr;
        ID3DBlob *vertexShaderBuffer = nullptr;
        ID3DBlob *pixelShaderBuffer = nullptr;

        if (FAILED(D3DCompileFromFile(vsFilename.c_str(), nullptr, nullptr, "TextureVertexShader", "vs_5_0",
                                      D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage)))
        {
            if (errorMessage)
            {
                OutputShaderErrorMessage(errorMessage, hWnd, vsFilename);
            }
            else
            {
                MessageBox(hWnd, std::filesystem::absolute(vsFilename).c_str(), L"Missing TextureVertexShader File", MB_OK);
            }

            return false;
        }

        if (FAILED(D3DCompileFromFile(psFilename.c_str(), nullptr, nullptr, "TexturePixelShader", "ps_5_0",
                                      D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage)))
        {
            if (errorMessage)
            {
                OutputShaderErrorMessage(errorMessage, hWnd, vsFilename);
            }
            else
            {
                MessageBox(hWnd, std::filesystem::absolute(vsFilename).c_str(), L"Missing TexturePixelShader File", MB_OK);
            }

            return false;
        }

        if (FAILED(device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, &m_vertexShader)))
        {
            MessageBox(hWnd, L"Could not create ID3D11VertexShader m_vertexShader", L"Error", MB_OK);
            return false;
        }

        if (FAILED(device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, &m_pixelShader)))
        {
            MessageBox(hWnd, L"Could not create ID3D11PixelShader m_pixelShader", L"Error", MB_OK);
            return false;
        }

        std::array<D3D11_INPUT_ELEMENT_DESC, 2> polygonLayout{};

        polygonLayout[0].SemanticName = "POSITION";
        polygonLayout[0].SemanticIndex = 0;
        polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
        polygonLayout[0].InputSlot = 0;
        polygonLayout[0].AlignedByteOffset = 0;
        polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        polygonLayout[0].InstanceDataStepRate = 0;

        polygonLayout[1].SemanticName = "TEXCOORD";
        polygonLayout[1].SemanticIndex = 0;
        polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
        polygonLayout[1].InputSlot = 0;
        polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
        polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        polygonLayout[1].InstanceDataStepRate = 0;

        uint32_t numElements = static_cast<uint32_t>(polygonLayout.size());

        if (FAILED(device->CreateInputLayout(polygonLayout.data(), numElements, vertexShaderBuffer->GetBufferPointer(),
                                             vertexShaderBuffer->GetBufferSize(), &m_inputLayout)))
        {
            MessageBox(hWnd, L"Could not create ID3D11InputLayout m_inputLayout", L"Error", MB_OK);
            return false;
        }

        vertexShaderBuffer->Release();
        vertexShaderBuffer = nullptr;
        pixelShaderBuffer->Release();
        pixelShaderBuffer = nullptr;
        errorMessage->Release();
        errorMessage = nullptr;

        D3D11_BUFFER_DESC matrixBufferDescription{};
        matrixBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
        matrixBufferDescription.ByteWidth = static_cast<uint32_t>(sizeof(MatrixBuffer_s));
        matrixBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        matrixBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        matrixBufferDescription.MiscFlags = 0;
        matrixBufferDescription.StructureByteStride = 0;

        if (FAILED(device->CreateBuffer(&matrixBufferDescription, nullptr, &m_matrixBuffer)))
        {
            MessageBox(hWnd, L"Could not create ID3D11Buffer m_matrixBuffer", L"Error", MB_OK);
            return false;
        }

        D3D11_SAMPLER_DESC samplerDescription{};
        samplerDescription.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerDescription.MipLODBias = 0.0f;
        samplerDescription.MaxAnisotropy = 1;
        samplerDescription.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        samplerDescription.BorderColor[0] = 0.0f;
        samplerDescription.BorderColor[1] = 0.0f;
        samplerDescription.BorderColor[2] = 0.0f;
        samplerDescription.BorderColor[3] = 0.0f;
        samplerDescription.MinLOD = 0.0f;
        samplerDescription.MaxLOD = D3D11_FLOAT32_MAX;

        if (FAILED(device->CreateSamplerState(&samplerDescription, &m_sampleState)))
        {
            MessageBox(hWnd, L"Could not create ID3D11SamplerState m_sampleState", L"Error", MB_OK);
            return false;
        }

        return true;
    }

} // namespace GP
