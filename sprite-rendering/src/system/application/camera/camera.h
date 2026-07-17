#pragma once

namespace GP
{

    class CCamera
    {
    public:
        CCamera();

        void Render();

        void SetPosition(XMFLOAT3 position) { m_position = position; }
        void SetRotation(XMFLOAT3 rotation) { m_rotation = rotation; }
        XMFLOAT3 GetPosition() const { return m_position; }
        XMFLOAT3 GetRotation() const { return m_rotation; }
        void GetViewMatrix(XMMATRIX &viewMatrix) const { viewMatrix = m_viewMatrix; }

    private:
        XMMATRIX m_viewMatrix;
        XMFLOAT3 m_position;
        XMFLOAT3 m_rotation;

        unsigned char _pad[8] = {};
    };

} // namespace GP
