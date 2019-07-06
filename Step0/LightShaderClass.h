//
// Created by lcy20 on 2019-07-05.
//

#ifndef STEP0_LIGHTSHADERCLASS_H
#define STEP0_LIGHTSHADERCLASS_H

///////////////////////////////////
// Class Name : LightShaderClass //
///////////////////////////////////

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <DirectXMath.h>

#include "AlignedAllocationPolicy.h"

using namespace DirectX;
using namespace std;

class LightShaderClass : public AlignedAllocationPolicy<16>
{
private:
    struct MatrixBufferType
    {
        XMMATRIX world;
        XMMATRIX view;
        XMMATRIX projection;
    };

    struct LightBufferType
    {
        XMFLOAT4 diffuseColor;
        XMFLOAT3 lightDirection;
        float padding; //Added extra padding so structure is a multiple of 16 for CreateBuffer function requirements.
    };

public:
    LightShaderClass();
    LightShaderClass(const LightShaderClass& other) = default;
    ~LightShaderClass() = default;

    bool Initialize(ID3D11Device* device, HWND hwnd);
    void Shutdown();
    bool Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor);

private:
    bool InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFileName, WCHAR* psFileName);
    void ShutdownShader();
    void OutputShaderErrorMessage(ID3DBlob* errorMessage, HWND hwnd, WCHAR* shaderFilename);

    bool SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor);
    void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
    ID3D11VertexShader* m_vertexShader;
    ID3D11PixelShader* m_pixelShader;
    ID3D11InputLayout* m_layout;
    ID3D11SamplerState* m_sampleState;
    ID3D11Buffer* m_matrixBuffer;
    ID3D11Buffer* m_lightBuffer;
};


#endif //STEP0_LIGHTSHADERCLASS_H
