#include "pch.h"

#include "system/system.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PSTR psCmdLine, _In_ int iCmdShow)
{
    GP::CSystem system{};

    if (system.Init())
    {
        system.Run();
    }

    system.Shutdown();
    return EXIT_SUCCESS;
}
