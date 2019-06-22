//
// Created by lcy20 on 2019-06-20.
//

#ifndef STEP0_CAMERACLASS_H
#define STEP0_CAMERACLASS_H

#include <DirectXMath.h>
using namespace DirectX;

class CameraClass
{
public:
    CameraClass();
    CameraClass(const CameraClass& other) = default;
    ~CameraClass() = default;

    void SetPosition(float x, float y, float z);
    void SetRotation(float x, float y, float z);

    XMFLOAT3 GetPosition();
    XMFLOAT3 GetRotation();

    void Render();
    void GetViewMatrix(XMMATRIX& viewMatrix);

private:
    XMFLOAT3 m_position;
    XMFLOAT3 m_rotation;
    XMMATRIX m_viewMatrix;
};


#endif //STEP0_CAMERACLASS_H
