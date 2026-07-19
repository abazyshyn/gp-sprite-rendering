#include "pch.h"

#include "application.h"

namespace GP
{

    bool CApplication::Init(int32_t windowWidth, int32_t windowHeight, HWND hWnd)
    {
        if (!m_Timer.Init())
        {
            MessageBox(hWnd, L"Could not initialize timer", L"Error", MB_OK);
            return false;
        }

        if (!m_Direct3D.Init(windowWidth, windowHeight, VSYNC_ENABLED, hWnd, FULL_SCREEN, FAR_PLANE, NEAR_PLANE))
        {
            MessageBox(hWnd, L"Could not initialize Direct3D", L"Error", MB_OK);
            return false;
        }

        m_Camera.SetPosition(0.0f, 0.0f, -1.0f);
        m_Camera.SetRotation(0.0f, 0.0f, 0.0f);

        // TODO: figure out how to improve this.
        if (!m_sprites[0].Init("sprite-back-temp.txt", m_Direct3D.GetDevice(), m_Direct3D.GetDeviceContext(), windowWidth, windowHeight, 0, 0, 800, 600))
        {
            MessageBox(hWnd, L"Could not initialize sprite", L"Error", MB_OK);
            return false;
        }
        if (!m_sprites[1].Init("spite-platform-temp.txt", m_Direct3D.GetDevice(), m_Direct3D.GetDeviceContext(), windowWidth, windowHeight, 0, 10, 800, 600))
        {
            MessageBox(hWnd, L"Could not initialize sprite", L"Error", MB_OK);
            return false;
        }
        if (!m_sprites[2].Init("sprite-knight.txt", m_Direct3D.GetDevice(), m_Direct3D.GetDeviceContext(), windowWidth, windowHeight, 300, 340, 256, 186))
        {
            MessageBox(hWnd, L"Could not initialize sprite", L"Error", MB_OK);
            return false;
        }

        if (!m_TextureShader.Init(m_Direct3D.GetDevice(), hWnd))
        {
            MessageBox(hWnd, L"Could not initialize texture shader", L"Error", MB_OK);
            return false;
        }

        return true;
    }

    void CApplication::Shutdown()
    {
        for (CSprite &sprite : m_sprites)
        {
            sprite.Shutdown();
        }
        m_TextureShader.Shutdown();
        m_Direct3D.Shutdown();
    }

    bool CApplication::Frame()
    {
        m_Timer.Frame();
        for (size_t i = 0; i < m_sprites.size(); ++i)
        {
            m_sprites[i].Update(m_Timer.GetFrameTime());
            m_sprites[i].UpdateTextureTranslation(i == 1);
        }

        if (!Render())
        {
            return false;
        }

        return true;
    }

    bool CApplication::Render()
    {
        m_Direct3D.BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

        m_Camera.Render();

        XMMATRIX worldMatrix{XMMatrixIdentity()};
        XMMATRIX viewMatrix{XMMatrixIdentity()};
        XMMATRIX orthoMatrix{XMMatrixIdentity()};
        m_Direct3D.GetWorldMatrix(worldMatrix);
        m_Camera.GetViewMatrix(viewMatrix);
        m_Direct3D.GetOrthoMatrix(orthoMatrix);

        m_Direct3D.TurnZBufferOff();
        m_Direct3D.TurnAlphaBlendingOn({0.0f, 0.0f, 0.0f, 0.0f});

        for (CSprite &sprite : m_sprites)
        {
            if (!sprite.Render(m_Direct3D.GetDeviceContext()))
            {
                return false;
            }

            m_TextureShader.SetShaderTextureTranslationBuffer(m_Direct3D.GetDeviceContext(), sprite.GetTextureTranslation());

            m_TextureShader.SetShaderMatrixBuffer(m_Direct3D.GetDeviceContext(), worldMatrix, viewMatrix, orthoMatrix);

            m_TextureShader.SetShaderTexture(m_Direct3D.GetDeviceContext(), sprite.GetTexture());

            if (!m_TextureShader.Render(m_Direct3D.GetDeviceContext(), sprite.GetIndexCount()))
            {
                return false;
            }
        }

        m_Direct3D.TurnZBufferOn();
        m_Direct3D.TurnAlphaBlendingOff();

        m_Direct3D.EndScene();
        return true;
    }

} // namespace GP
