//
// Created by lcy20 on 2019-07-06.
//

#include <DirectXMath.h>

#include "LightClass.h"

using namespace DirectX;

void LightClass::SetDiffuseColor(float red, float green, float blue, float alpha)
{
    m_diffuseColor = XMFLOAT4(red, green, blue, alpha);
    return;
}

void LightClass::SetDirection(float x, float y, float z)
{
    m_direction = XMFLOAT3(x, y, z);
    return;
}

DirectX::XMFLOAT4 LightClass::GetDiffuseColor()
{
    return m_diffuseColor;
}

DirectX::XMFLOAT3 LightClass::GetDirection()
{
    return m_direction;
}
