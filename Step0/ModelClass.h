//
// Created by lcy20 on 2019-06-20.
//

#ifndef STEP0_MODELCLASS_H
#define STEP0_MODELCLASS_H
//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <DirectXMath.h>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "TextureClass.h"
#include "AlignedAllocationPolicy.h"

using namespace DirectX;

class ModelClass : public AlignedAllocationPolicy<16>
{
private:
    struct VertexType
    {
        XMFLOAT3 position;
        XMFLOAT2 texture;
    };
public:
    ModelClass();
    ModelClass(const ModelClass& other) = default;
    ~ModelClass() = default;

    bool Initialize(ID3D11Device *device, ID3D11DeviceContext *deviceContext, char *textureFileName);
    void Shutdown();
    void Render(ID3D11DeviceContext* deviceContext);

    int GetIndexCount();

    ID3D11ShaderResourceView* GetTexture();

private:
    bool InitializeBuffers(ID3D11Device* device);
    void ShutdownBuffers();
    void RenderBuffers(ID3D11DeviceContext* deviceContext);

    bool LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* textureFileName);
    void ReleaseTexture();

private:
    ID3D11Buffer* m_vertexBuffer = nullptr;
    ID3D11Buffer* m_indexBuffer = nullptr;
    int m_vertexCount = 0;
    int m_indexCount = 0;

    TextureClass* m_Texture;
};


#endif //STEP0_MODELCLASS_H
