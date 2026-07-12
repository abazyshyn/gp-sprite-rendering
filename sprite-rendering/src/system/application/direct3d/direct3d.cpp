#include "pch.h"

#include "direct3d.h"

namespace GP
{

    CDirect3D::CDirect3D()
        : m_swapChain(nullptr),
          m_device(nullptr),
          m_deviceContext(nullptr),
          m_renderTargetView(nullptr),
          m_depthStencilBuffer(nullptr),
          m_depthStencilState(nullptr),
          m_depthStencilView(nullptr),
          m_rasterizerState(nullptr),
          m_depthDisabledStencilState(nullptr)
    {
    }

    bool CDirect3D::Init(int32_t windowWidth, int32_t windowHeight, bool vsync, HWND hWnd, bool fullscreen, float nearPlane, float farPlane)
    {
        m_vsync = vsync;

        IDXGIFactory *factory = nullptr;
        if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void **>(&factory))))
        {
            MessageBox(hWnd, L"Could not create IDXGIFactory", L"Error", MB_OK);
            return false;
        };

        IDXGIAdapter *adapter = nullptr;
        if (FAILED(factory->EnumAdapters(0, &adapter)))
        {
            MessageBox(hWnd, L"Could not enumerate adapters", L"Error", MB_OK);
            return false;
        }

        IDXGIOutput *adapterOutput = nullptr;
        if (FAILED(adapter->EnumOutputs(0, &adapterOutput)))
        {
            MessageBox(hWnd, L"Could not enumerate output adapters", L"Error", MB_OK);
            return false;
        }

        uint32_t countModes{};
        if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &countModes, nullptr)))
        {
            MessageBox(hWnd, L"Could not enumerate display modes", L"Error", MB_OK);
            return false;
        }

        std::vector<DXGI_MODE_DESC> displayModes{};
        if (FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &countModes, displayModes.data())))
        {
            MessageBox(hWnd, L"Could not retrieve display modes", L"Error", MB_OK);
            return false;
        }

        uint32_t refreshRateNumenator{};
        uint32_t refreshRateDenominator{};
        for (const DXGI_MODE_DESC &displayMode : displayModes)
        {
            if (displayMode.Width == static_cast<uint32_t>(windowWidth) && displayMode.Height == static_cast<uint32_t>(windowHeight))
            {
                refreshRateNumenator = displayMode.RefreshRate.Numerator;
                refreshRateDenominator = displayMode.RefreshRate.Denominator;
                break;
            }
        }

        DXGI_ADAPTER_DESC adapterDescription{};
        if (FAILED(adapter->GetDesc(&adapterDescription)))
        {
            MessageBox(hWnd, L"Could not retrieve adapter description", L"Error", MB_OK);
            return false;
        }

        constexpr size_t SZ_KILOBYTES = 1024;
        constexpr size_t SZ_MEGABYTES = 1024;
        m_videoCardMemory = static_cast<int32_t>(adapterDescription.DedicatedVideoMemory / SZ_KILOBYTES / SZ_MEGABYTES);

        size_t stringLength{};
        if (wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDescription.Description, 128))
        {
            MessageBox(hWnd, L"Could not store adapter description", L"Error", MB_OK);
            return false;
        }

        adapterOutput->Release();
        adapterOutput = nullptr;

        adapter->Release();
        adapter = nullptr;

        factory->Release();
        factory = nullptr;

        DXGI_SWAP_CHAIN_DESC swapChainDescription{};
        swapChainDescription.BufferCount = 1;
        swapChainDescription.BufferDesc.Width = windowWidth;
        swapChainDescription.BufferDesc.Height = windowHeight;
        swapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDescription.BufferDesc.RefreshRate.Numerator = m_vsync ? refreshRateNumenator : 0;
        swapChainDescription.BufferDesc.RefreshRate.Denominator = m_vsync ? refreshRateDenominator : 0;
        swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDescription.OutputWindow = hWnd;
        swapChainDescription.SampleDesc.Count = 1;   // Turn multisampling off
        swapChainDescription.SampleDesc.Quality = 0; //
        swapChainDescription.Windowed = !fullscreen;
        swapChainDescription.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        swapChainDescription.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        swapChainDescription.Flags = 0;

        D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

        if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &featureLevel, 1, D3D11_SDK_VERSION,
                                                 &swapChainDescription, &m_swapChain, &m_device, nullptr, &m_deviceContext)))
        {
            MessageBox(hWnd, L"Could not create IDXGISwapChain", L"Error", MB_OK);
            return false;
        }

        ID3D11Texture2D *backBuffer = nullptr;
        if (FAILED(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&backBuffer))))
        {
            MessageBox(hWnd, L"Could not retrieve ID3D11Texture2D backBuffer from swap chain", L"Error", MB_OK);
            return false;
        }

        if (FAILED(m_device->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView)))
        {
            MessageBox(hWnd, L"Could not create ID3D11RenderTargetView", L"Error", MB_OK);
            return false;
        }

        backBuffer->Release();
        backBuffer = nullptr;

        D3D11_TEXTURE2D_DESC depthStencilBufferDescription{};
        depthStencilBufferDescription.Width = static_cast<uint32_t>(windowWidth);
        depthStencilBufferDescription.Height = static_cast<uint32_t>(windowHeight);
        depthStencilBufferDescription.MipLevels = 1;
        depthStencilBufferDescription.ArraySize = 1;
        depthStencilBufferDescription.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        depthStencilBufferDescription.SampleDesc.Count = 1;
        depthStencilBufferDescription.SampleDesc.Quality = 0;
        depthStencilBufferDescription.Usage = D3D11_USAGE_DEFAULT;
        depthStencilBufferDescription.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        depthStencilBufferDescription.CPUAccessFlags = 0;
        depthStencilBufferDescription.MiscFlags = 0;

        if (FAILED(m_device->CreateTexture2D(&depthStencilBufferDescription, nullptr, &m_depthStencilBuffer)))
        {
            MessageBox(hWnd, L"Could not create ID3D11Texture2D m_depthStencilBuffer", L"Error", MB_OK);
            return false;
        }

        D3D11_DEPTH_STENCIL_DESC depthStencilDescription{};
        depthStencilDescription.DepthEnable = true;
        depthStencilDescription.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        depthStencilDescription.DepthFunc = D3D11_COMPARISON_LESS;
        depthStencilDescription.StencilEnable = true;
        depthStencilDescription.StencilReadMask = 0xFF;
        depthStencilDescription.StencilWriteMask = 0xFF;
        depthStencilDescription.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        depthStencilDescription.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
        depthStencilDescription.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        depthStencilDescription.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        depthStencilDescription.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        depthStencilDescription.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
        depthStencilDescription.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        depthStencilDescription.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        if (FAILED(m_device->CreateDepthStencilState(&depthStencilDescription, &m_depthDisabledStencilState)))
        {
            MessageBox(hWnd, L"Could not create ID3D11DepthStencilState m_depthDisabledStencilState", L"Error", MB_OK);
            return false;
        }

        return true;
    }

} // namespace GP
