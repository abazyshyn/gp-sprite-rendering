#pragma once

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "direct3d/direct3d.h"
#include "shaders/texture_shader.h"
#include "sprite/sprite.h"

namespace GP
{
    /////////////
    // GLOBALS //
    /////////////
    constexpr bool FULL_SCREEN = false;
    constexpr bool SHOW_CURSOR = true;
    constexpr bool VSYNC_ENABLED = true;
    constexpr float FAR_PLANE = 1000.0f;
    constexpr float NEAR_PLANE = 0.3f;

    class CApplication
    {
    public:
        bool Init(int32_t windowWidth, int32_t windowHeight, HWND hWnd);
        void Shutdown();
        bool Frame();

    private:
        bool Render();

    private:
        CDirect3D m_Direct3D;
        CTextureShader m_TextureShader;
        CSprite m_Sprite;
    };

} // namespace GP
