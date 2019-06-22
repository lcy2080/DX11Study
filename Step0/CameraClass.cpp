//
// Created by lcy20 on 2019-06-20.
//

#include "CameraClass.h"

CameraClass::CameraClass()
{
    m_position = XMFLOAT3(0.f, 0.f, 0.f);
    m_rotation = XMFLOAT3(0.f, 0.f, 0.f);
}

void CameraClass::SetPosition(float x, float y, float z)
{
    m_position.x = x;
    m_position.y = y;
    m_position.z = z;
}

void CameraClass::SetRotation(float x, float y, float z)
{
    m_rotation.x = x;
    m_rotation.y = y;
    m_rotation.z = z;
}

XMFLOAT3 CameraClass::GetPosition()
{
    return m_position;
}

XMFLOAT3 CameraClass::GetRotation()
{
    return m_rotation;
}

void CameraClass::Render()
{
    XMFLOAT3 up, position, lookAt;
    XMVECTOR upVector, positionVector, lookAtVector;
    float yaw, pitch, roll;
    XMMATRIX rotationMatrix;

    //set up vector
    up.x = 0.0f;
    up.y = 1.0f;
    up.z = 0.0f;

    //load up vector to XMVECTOR struct
    upVector = XMLoadFloat3(&up);

    //set camera's position 3D world
    position = m_position;

    //load position vector to XMVECTOR struct
    positionVector = XMLoadFloat3(&position);

    //set camera's look direction
    lookAt.x = 0.f;
    lookAt.y = 0.f;
    lookAt.z = 1.f;

    //load look direction to XMVECTOR struct
    lookAtVector = XMLoadFloat3(&lookAt);

    // yaw (Y axis), pitch (X axis), roll (Z axis) as radian.
    pitch = m_rotation.x * 0.0174532925f;
    yaw = m_rotation.y * 0.0174532925f;
    roll = m_rotation.z * 0.0174532925f;

    // make rotation matrix through yaw, pitch, roll
    rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

    //setup camera's position on origin as set look...
    lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
    upVector = XMVector3TransformCoord(upVector, rotationMatrix);

    //rotated camera set on viewer position
    lookAtVector = XMVectorAdd(positionVector, lookAtVector);

    //make view matrix
    m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}

void CameraClass::GetViewMatrix(XMMATRIX &viewMatrix)
{
    viewMatrix = m_viewMatrix;
}




