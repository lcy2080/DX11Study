//
// Created by lcy20 on 2019-06-20.
//

#ifndef STEP0_MODELCLASS_H
#define STEP0_MODELCLASS_H

#include <d3d11.h>
#include <DirectXMath.h>
#include "AlignedAllocationPolicy.h"

using namespace DirectX;

class ModelClass : public AlignedAllocationPolicy<16>
{
private:
    struct VertexType
    {
        XMFLOAT3 position;
        XMFLOAT4 color;
    };
public:
    ModelClass() = default;
    ModelClass(const ModelClass& other) = default;
    ~ModelClass() = default;

    bool Initialize(ID3D11Device* device);
    void Shutdown();
    void Render(ID3D11DeviceContext* deviceContext);

    int GetIndexCount();

private:
    bool InitializeBuffers(ID3D11Device* device);
    void ShutdownBuffers();
    void RenderBuffers(ID3D11DeviceContext* deviceContext);

private:
    ID3D11Buffer* m_vertexBuffer = nullptr;
    ID3D11Buffer* m_indexBuffer = nullptr;
    int m_vertexCount = 0;
    int m_indexCount = 0;
};


#endif //STEP0_MODELCLASS_H
