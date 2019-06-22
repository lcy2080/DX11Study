//
// Created by lcy20 on 2019-06-18.
//

#include <d3dcompiler.h>
#include "ColorShaderClass.h"
using namespace DirectX;

bool ColorShaderClass::Initialize(ID3D11Device *device, HWND hwnd)
{
    return InitializeShader(device, hwnd, L"E:/Git/DX11/Step0/Color.vs", L"E:/Git/DX11/Step0/Color.ps");
}

void ColorShaderClass::Shutdown()
{
    ShutdownShader();
}

bool ColorShaderClass::Render(ID3D11DeviceContext *deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
                         XMMATRIX projectionMatrix)
{
    //set parameter for render
    if (!SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix))
    {
        return false;
    }

    // now render the prepared buffers with the shader.
    RenderShader(deviceContext, indexCount);

    return true;
}

bool ColorShaderClass::InitializeShader(ID3D11Device *device, HWND hwnd, WCHAR *vsFilename, WCHAR *psFilename)
{
    ID3DBlob* errorMessage = nullptr;
    DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
    //compile vertex shader code.
    ID3DBlob* vertexShaderBuffer = nullptr;
    if (FAILED(D3DCompileFromFile(vsFilename, NULL, NULL, "ColorVertexShader", "vs_5_0", flags, 0, &vertexShaderBuffer, &errorMessage)))
    {
        //show error when fail to compile
        if (errorMessage)
        {
            OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
        }
        else
        { //other message
            MessageBoxW(hwnd, vsFilename, L"Missing Shader File", MB_OK);
        }
        return false;
    }

    //compile pixel shader code.
    ID3DBlob* pixelShaderBuffer = nullptr;
    if (FAILED(D3DCompileFromFile(psFilename, NULL, NULL, "ColorPixelShader", "ps_5_0", flags, 0, &pixelShaderBuffer, &errorMessage)))
    {
        //show error message when fail to compile
        if (errorMessage)
        {
            OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
        }
        else
        { //can not find shader file
            MessageBoxW(hwnd, psFilename, L"Missing Shader File", MB_OK);
        }
        return false;
    }

    //generate vertex shader on buffer.
    if (FAILED(device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader)))
    {
        return false;
    }

    //generate pixel shader on buffer.
    if (FAILED(device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader)))
    {
        return false;
    }

    //setup vertex input layout struct.
    D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
    polygonLayout[0].SemanticName = "POSITION";
    polygonLayout[0].SemanticIndex = 0;
    polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[0].InputSlot = 0;
    polygonLayout[0].AlignedByteOffset = 0;
    polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[0].InstanceDataStepRate = 0;

    polygonLayout[1].SemanticName = "COLOR";
    polygonLayout[1].SemanticIndex = 0;
    polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    polygonLayout[1].InputSlot = 0;
    polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[1].InstanceDataStepRate = 0;

    //get layout element number
    unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

    //make vertex input layout
    if (FAILED(device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout)))
    {
        return false;
    }

    //release vertex shader buffer/pixel shader buffer.
    vertexShaderBuffer->Release();
    vertexShaderBuffer = nullptr;

    pixelShaderBuffer->Release();
    pixelShaderBuffer = nullptr;

    //implement array constant buffer struct on vertex
    D3D11_BUFFER_DESC matrixBufferDesc;
    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
    matrixBufferDesc.StructureByteStride = 0;

    //make constant buffer ptr
    if(FAILED(device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer)))
    {
        return false;
    }
    return true;
}

void ColorShaderClass::ShutdownShader()
{
    //array constant buffer release
    if (m_matrixBuffer)
    {
        m_matrixBuffer->Release();
        m_matrixBuffer = nullptr;
    }
    //release layout
    if (m_layout)
    {
        m_layout->Release();
        m_layout = nullptr;
    }
    //release pixel shader
    if(m_pixelShader)
    {
        m_pixelShader->Release();
        m_pixelShader = nullptr;
    }
    //release vertex shader
    if(m_vertexShader)
    {
        m_vertexShader->Release();
        m_vertexShader = nullptr;
    }
}

void ColorShaderClass::OutputShaderErrorMessage(ID3DBlob *errorMessage, HWND hwnd, WCHAR *shaderFileName)
{
	char* compileErrors;
	unsigned long long bufferSize;
	ofstream fout;

	//Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	//Get the length of te message.
	bufferSize = errorMessage->GetBufferSize();

	//Open a file to write the error message to.
	fout.open("shader-error.txt");

	//Write out the error message
	fout.write(compileErrors, bufferSize);

	// close the fire
	fout.close();
	
    //show error message
    OutputDebugStringA(reinterpret_cast<const char*>(errorMessage->GetBufferPointer()));

    //release error message
    errorMessage->Release();
    errorMessage = nullptr;

    //compile error to popup message
    MessageBoxW(hwnd, L"Error compiling shader.", shaderFileName, MB_OK);
}

bool ColorShaderClass::SetShaderParameters(ID3D11DeviceContext *deviceContext, XMMATRIX worldMatrix,
        XMMATRIX viewMatrix, XMMATRIX projectionMatrix)
{
    //...
    worldMatrix = XMMatrixTranspose(worldMatrix);
    viewMatrix = XMMatrixTranspose(viewMatrix);
    projectionMatrix = XMMatrixTranspose(projectionMatrix);

    //lock constant buffer to write
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    if (FAILED(deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
    {
        return false;
    }

    //get pointer about constant buffer data
    MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;

    //copy to constant buffer
    dataPtr->world = worldMatrix;
    dataPtr->view = viewMatrix;
    dataPtr->projection = projectionMatrix;

    //release lock
    deviceContext->Unmap(m_matrixBuffer, 0);

    //set constant buffer position on vertex shader
    unsigned bufferNumber = 0;

    //change vertex shader's constant buffer to changed value
    deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
    return true;
}

void ColorShaderClass::RenderShader(ID3D11DeviceContext *deviceContext, int indexCount)
{
    //set vertex input layout
    deviceContext->IASetInputLayout(m_layout);

    //setup vertex shader and pixel shader to draw triangle
    deviceContext->VSSetShader(m_vertexShader, NULL, 0);
    deviceContext->PSSetShader(m_pixelShader, NULL, 0);

    //draw triangle
    deviceContext->DrawIndexed(indexCount, 0, 0);

	return;
}
