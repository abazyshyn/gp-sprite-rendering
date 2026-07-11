#pragma once

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "application/application.h"

namespace GP
{

    class CSystem
    {
    public:
        CSystem();

        bool Init();
        void Run();
        void Shutdown();

        LRESULT CALLBACK MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    private:
        bool Frame();
        void InitWindows(int32_t &windowWidth, int32_t windowHeight);
        void ShutdownWindows();

    private:
        HWND m_hWnd;
        HINSTANCE m_hInstance;
        LPCWSTR m_applicationName;
        // CApplication m_Application;
    };

    /////////////
    // GLOBALS //
    /////////////
    static CSystem *applicationHandler = nullptr;

    /////////////////////////
    // FUNCTION PROTOTYPES //
    /////////////////////////
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

} // namespace GP
