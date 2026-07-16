#pragma once

namespace GP
{

    class CTextureShader
    {
    public:
        CTextureShader();

        bool Init(ID3D11Device *device, HWND hWnd);
        void Shutdown();
        bool Render(ID3D11DeviceContext *deviceContext, ID3D11ShaderResourceView *texture, int32_t indexCount,
                    XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);

    private:
        bool InitShader(ID3D11Device *device, HWND hWnd, const std::filesystem::path &vsFilename, const std::filesystem::path &psFilename);
        void ShutdownShader();
        void OutputShaderErrorMessage(ID3D10Blob *errorMessage, HWND hWnd, const std::filesystem::path &shaderFilename);
        // set shader params func
        void RenderShader(ID3D11DeviceContext *deviceContext, int32_t indexCount);

    private:
        struct MatrixBuffer_s
        {
            XMMATRIX worldMatrix;
            XMMATRIX viewMatrix;
            XMMATRIX projectionMatrix;
        };

        ID3D11VertexShader *m_vertexShader;
        ID3D11PixelShader *m_pixelShader;
        ID3D11InputLayout *m_inputLayout;
        ID3D11Buffer *m_matrixBuffer;
        ID3D11SamplerState *m_sampleState;
    };

} // namespace GP
