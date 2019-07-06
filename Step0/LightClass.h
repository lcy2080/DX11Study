//
// Created by lcy20 on 2019-07-06.
//

#ifndef STEP0_LIGHTCLASS_H
#define STEP0_LIGHTCLASS_H

//////////////
// INCLUDES //
//////////////

namespace DirectX
{
    struct XMFLOAT4;
    struct XMFLOAT3;
}

class LightClass
{
public:
    LightClass() = default;
    LightClass(const LightClass&) = default;
    ~LightClass() = default;

    void SetDiffuseColor(float red, float green, float blue, float alpha);
    void SetDirection(float x, float y, float z);

    DirectX::XMFLOAT4 GetDiffuseColor();
    DirectX::XMFLOAT3 GetDirection();

private:
    DirectX::XMFLOAT4 m_diffuseColor;
    DirectX::XMFLOAT3 m_direction;
};


#endif //STEP0_LIGHTCLASS_H
