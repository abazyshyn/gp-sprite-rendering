#include "pch.h"

#include "camera.h"

namespace GP
{

    CCamera::CCamera()
        : m_viewMatrix(XMMatrixIdentity()),
          m_position(0.0f, 0.0f, 0.0f),
          m_rotation(0.0f, 0.0f, 0.0f)
    {
    }

    void CCamera::Render()
    {
        XMFLOAT3 up{0.0f, 1.0f, 0.0f};
        XMVECTOR upVector{XMLoadFloat3(&up)};

        XMFLOAT3 position{m_position};
        XMVECTOR positionVector{XMLoadFloat3(&position)};

        XMFLOAT3 lookAt{0.0f, 0.0f, 1.0f};
        XMVECTOR lookAtVector{XMLoadFloat3(&lookAt)};

        float pitch = XMConvertToRadians(m_rotation.x);
        float yaw = XMConvertToRadians(m_rotation.y);
        float roll = XMConvertToRadians(m_rotation.z);
        XMMATRIX rotationMatrix{XMMatrixRotationRollPitchYaw(pitch, yaw, roll)};

        lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
        upVector = XMVector3TransformCoord(upVector, rotationMatrix);

        lookAtVector = XMVectorAdd(lookAtVector, positionVector);
        m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
    }

} // namespace GP
