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

        m_Camera.SetPosition(0.0f, 0.0f, 0.0f);
        m_Camera.SetRotation(0.0f, 0.0f, 0.0f);

        // TODO: should be array of sprites
        if (!m_Sprite.Init("sprite-data-01.txt", m_Direct3D.GetDevice(), m_Direct3D.GetDeviceContext(), windowWidth, windowHeight, 50, 50))
        {
            MessageBox(hWnd, L"Could not initialize sprite", L"Error", MB_OK);
            return false;
        }

        if (!m_TextureShader.Init(m_Direct3D.GetDevice(), hWnd))
        {
            MessageBox(hWnd, L"Could not initialize texture shader", L"Error", MB_OK);
            return false;
        }
    }

    void CApplication::Shutdown()
    {
        m_Sprite.Shutdown();
        m_TextureShader.Shutdown();
        m_Direct3D.Shutdown();
    }

    bool CApplication::Frame()
    {
        m_Timer.Frame();
        m_Sprite.Update(m_Timer.GetFrameTime());

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

        if (!m_Sprite.Render(m_Direct3D.GetDeviceContext()))
        {
            return false;
        }

        if (!m_TextureShader.Render(m_Direct3D.GetDeviceContext(), m_Sprite.GetIndexCount()))
        {
            return false;
        }

        m_Direct3D.TurnZBufferOn();

        m_Direct3D.EndScene();
        return true;
    }

} // namespace GP
