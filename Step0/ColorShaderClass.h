//
// Created by lcy20 on 2019-06-18.
//

#ifndef STEP0_COLORSHADERCLASS_H
#define STEP0_COLORSHADERCLASS_H

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>
#include "AlignedAllocationPolicy.h"

using namespace DirectX;
using namespace std;

class ColorShaderClass : public AlignedAllocationPolicy<16>
{
private:
    struct MatrixBufferType
    {
        XMMATRIX world;
        XMMATRIX view;
        XMMATRIX projection;
    };
public:
    ColorShaderClass() = default;
    ColorShaderClass(const ColorShaderClass& other) = default;
    ~ColorShaderClass() = default;

    bool Initialize(ID3D11Device* device, HWND hwnd);
    void Shutdown();
    bool Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);

private:
    bool InitializeShader(ID3D11Device *device, HWND hwnd, WCHAR *vsFilename, WCHAR *psFilename);
    void ShutdownShader();
    void OutputShaderErrorMessage(ID3DBlob *errorMessage, HWND hwnd, WCHAR *shaderFileName);

    bool SetShaderParameters(ID3D11DeviceContext *deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix);
    void RenderShader(ID3D11DeviceContext* deviceContext, int indexCount);

private:
    ID3D11VertexShader* m_vertexShader = nullptr;
    ID3D11PixelShader* m_pixelShader = nullptr;
    ID3D11InputLayout* m_layout = nullptr;
    ID3D11Buffer* m_matrixBuffer = nullptr;
};

#endif //STEP0_COLORSHADERCLASS_H