#include "pch.h"

#include "system.h"

namespace GP
{

    CSystem::CSystem()
        : m_hWnd(nullptr),
          m_hInstance(nullptr),
          m_applicationName(nullptr)
    {
    }

    bool CSystem::Init()
    {
        int32_t windowWidth{};
        int32_t windowHeight{};
        InitWindows(windowWidth, windowHeight);

        // Init app

        return true;
    }

    void CSystem::Run()
    {
        MSG msg{};

        for (;;)
        {
            if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }

            if (msg.message == WM_QUIT)
            {
                break;
            }

            if (!Frame())
            {
                break;
            }
        }
    }

    void CSystem::Shutdown()
    {
        ShutdownWindows();
    }

    LRESULT CSystem::MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
            default:
            {
                return DefWindowProc(hWnd, uMsg, wParam, lParam);
            }
        }

        return 0;
    }

    bool CSystem::Frame()
    {
        // if (!m_Application.Frame())
        // {
        //     return false;
        // }

        return true;
    }

    void CSystem::InitWindows(int32_t &windowWidth, int32_t windowHeight)
    {
        applicationHandler = this;

        m_hInstance = GetModuleHandle(nullptr);
        m_applicationName = L"Sprite Rendering";

        WNDCLASSEX wc{};
        wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wc.lpfnWndProc = WndProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = m_hInstance;
        wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
        wc.hIconSm = wc.hIcon;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
        wc.lpszMenuName = nullptr;
        wc.lpszClassName = m_applicationName;
        wc.cbSize = sizeof(WNDCLASSEX);

        RegisterClassEx(&wc);

        windowWidth = GetSystemMetrics(SM_CXSCREEN);
        windowHeight = GetSystemMetrics(SM_CYSCREEN);

        int32_t posX{};
        int32_t posY{};
        if (FULL_SCREEN)
        {
            DEVMODE dmScreenSettings{};
            dmScreenSettings.dmSize = sizeof(dmScreenSettings);
            dmScreenSettings.dmPelsWidth = static_cast<unsigned long>(windowWidth);
            dmScreenSettings.dmPelsHeight = static_cast<unsigned long>(windowHeight);
            dmScreenSettings.dmBitsPerPel = 32;
            dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

            ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

            // Set the position of the window to the top left corner.
            posX = 0;
            posY = 0;
        }
        else
        {
            windowWidth = 800;
            windowHeight = 600;

            // Place the window in the middle of the screen.
            posX = (GetSystemMetrics(SM_CXSCREEN) - windowWidth) >> 1;
            posY = (GetSystemMetrics(SM_CYSCREEN) - windowHeight) >> 1;
        }

        m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
                                posX, posY, windowWidth, windowHeight, nullptr, nullptr, m_hInstance, nullptr);
        ShowWindow(m_hWnd, SW_SHOW);
        SetForegroundWindow(m_hWnd);
        SetFocus(m_hWnd);
        ShowCursor(SHOW_CURSOR);
    }

    void CSystem::ShutdownWindows()
    {
        ShowCursor(SHOW_CURSOR);

        if (FULL_SCREEN)
        {
            ChangeDisplaySettings(nullptr, 0);
        }

        DestroyWindow(m_hWnd);
        m_hWnd = nullptr;

        UnregisterClass(m_applicationName, m_hInstance);
        m_hInstance = nullptr;
    }

    LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
            case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            }
            case WM_CLOSE:
            {
                PostQuitMessage(0);
                return 0;
            }
            default:
            {
                return applicationHandler->MessageHandler(hWnd, uMsg, wParam, lParam);
            }
        }
    }

} // namespace GP
