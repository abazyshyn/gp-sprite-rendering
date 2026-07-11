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

        return true;
    }

} // namespace GP
