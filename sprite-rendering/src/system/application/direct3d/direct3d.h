#pragma once

/////////////
// LINKING //
/////////////
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace GP
{

    class CDirect3D
    {
    public:
        CDirect3D();

        bool Init(int32_t windowWidth, int32_t windowHeight, bool vsync, HWND hWnd, bool fullscreen, float nearPlane, float farPlane);
        void Shutdown();

        void BeginScene(float red, float green, float blue, float alpha);
        void EndScene();

        ID3D11Device *GetDevice() { return m_device; };
        ID3D11DeviceContext *GetDeviceContext() { return m_deviceContext; };

        void GetWorldMatrix(XMMATRIX &worldMatrix) const { worldMatrix = m_worldMatrix; };
        void GetOrthoMatrix(XMMATRIX &orthoMatrix) const { orthoMatrix = m_orthoMatrix; };
        void GetProjectionMatrix(XMMATRIX &projectionMatrix) const { projectionMatrix = m_projectionMatrix; };

        void GetVideoCardInfo(char *cardName, int32_t &memory) const;
        void SetBackBufferRenderTarget() { m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView); }
        void ResetViewport() { m_deviceContext->RSSetViewports(1, &m_viewport); }

        void TurnZBufferOn() { m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1); }
        void TurnZBufferOff() { m_deviceContext->OMSetDepthStencilState(m_depthDisabledStencilState, 1); }
        void TurnAlphaBlendingOn(std::array<float, 4> blendFactor) { m_deviceContext->OMSetBlendState(m_alphaBlendState, blendFactor.data(), 0xFFFFFFFF); }
        void TurnAlphaBlendingOff() { m_deviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF); }

    private:
        IDXGISwapChain *m_swapChain;
        ID3D11Device *m_device;
        ID3D11DeviceContext *m_deviceContext;
        ID3D11RenderTargetView *m_renderTargetView;
        ID3D11Texture2D *m_depthStencilBuffer;
        ID3D11DepthStencilState *m_depthStencilState;
        ID3D11DepthStencilView *m_depthStencilView;
        ID3D11BlendState *m_alphaBlendState;
        ID3D11RasterizerState *m_rasterizerState;
        ID3D11DepthStencilState *m_depthDisabledStencilState;
        D3D11_VIEWPORT m_viewport;
        XMMATRIX m_worldMatrix;
        XMMATRIX m_orthoMatrix;
        XMMATRIX m_projectionMatrix;
        char m_videoCardDescription[128];
        int32_t m_videoCardMemory;
        bool m_vsync;

        unsigned char _padding[11] = {};
    };

} // namespace GP
