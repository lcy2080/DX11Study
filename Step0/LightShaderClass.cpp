//
// Created by lcy20 on 2019-07-05.
//

#include <d3dcompiler.h>
#include <fstream>

#include "LightShaderClass.h"

LightShaderClass::LightShaderClass()
    : m_vertexShader(nullptr)
    , m_pixelShader(nullptr)
    , m_layout(nullptr)
    , m_sampleState(nullptr)
    , m_matrixBuffer(nullptr)
{

}

bool LightShaderClass::Initialize(ID3D11Device *device, HWND hwnd)
{
    bool result;

    //Initialize the vertex and pixel shaders.
    result = InitializeShader(device, hwnd, L"E:/Git/Dx11/Step0/Light.vs", L"E:/Git/Dx11/Step0/Light.ps");
    if (!result)
    {
        return false;
    }
    return true;
}

void LightShaderClass::Shutdown()
{
    //Shutdown the vertex and pixel shaders as well as the related objects.
    ShutdownShader();

    return;
}

bool LightShaderClass::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor)
{
    bool result;

    //Set the shader parameters that it will use for rendering.
    result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture, lightDirection, diffuseColor);
    if (!result)
    {
        return false;
    }

    //Now render the prepared buffers with the shader.
    RenderShader(deviceContext, indexCount);

    return true;
}

bool LightShaderClass::InitializeShader(ID3D11Device *device, HWND hwnd, WCHAR *vsFileName, WCHAR *psFileName)
{
    HRESULT result;
    ID3DBlob* errorMessage = nullptr;
    ID3DBlob* vertexShaderBuffer = nullptr;
    ID3DBlob* pixelShaderBuffer = nullptr;

    DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

    //Compile the vertex shader code.
    result = D3DCompileFromFile(vsFileName, nullptr, nullptr, "LightVertexShader", "vs_5_0", flags, 0, &vertexShaderBuffer, &errorMessage);
    if (FAILED(result))
    {
        //If the shader failed to compile it should have writen something to the error message.
        if(errorMessage)
        {
            OutputShaderErrorMessage(errorMessage, hwnd, vsFileName);
        }
        else
        { // If there was nothing in the error message then it simply could not find the shader file itself.
            MessageBoxW(hwnd, vsFileName, L"Missing Shader File", MB_OK);
        }
        return false;
    }

	//Compile the pixel shader code.
	result = D3DCompileFromFile(psFileName, nullptr, nullptr, "LightPixelShader", "ps_5_0", flags, 0, &pixelShaderBuffer, &errorMessage);
	if (FAILED(result))
	{
		//If the shader failed to compile it should have writen something to the error message.
		if (errorMessage)
		{
			OutputShaderErrorMessage(errorMessage, hwnd, psFileName);
		}
		else
		{ // If there was nothing in the error message then it simply could not find the shader file itself.
			MessageBoxW(hwnd, psFileName, L"Missing Shader File", MB_OK);
		}
		return false;
	}

	//Create the vertex shader from the buffer.
	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, &m_vertexShader);
	if (FAILED(result))
	{
		return false;
	}

    //Create the pixel shader from the buffer.
    result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, &m_pixelShader);
    if (FAILED(result))
    {
        return false;
    }

    //Create the vertex input layout description.
    //This setup needs to match the VertexType structure in the ModelClass and in the shader.
    D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
    polygonLayout[0].SemanticName = "POSITION";
    polygonLayout[0].SemanticIndex = 0;
    polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[0].InputSlot = 0;
    polygonLayout[0].AlignedByteOffset = 0;
    polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[0].InstanceDataStepRate = 0;

    polygonLayout[1].SemanticName = "TEXCOORD";
    polygonLayout[1].SemanticIndex = 0;
    polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
    polygonLayout[1].InputSlot = 0;
    polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[1].InstanceDataStepRate = 0;

    polygonLayout[2].SemanticName = "NORMAL";
    polygonLayout[2].SemanticIndex = 0;
    polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[2].InputSlot = 0;
    polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    polygonLayout[2].InstanceDataStepRate = 0;

    //Get a count of the elements in the layout.
    unsigned int numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

    //Create the vertex input layout.
    result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_layout);
    if (FAILED(result))
    {
        return false;
    }

    //Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
    vertexShaderBuffer->Release();
    vertexShaderBuffer = nullptr;

    pixelShaderBuffer->Release();
    pixelShaderBuffer = nullptr;

    //Create a texture sampler state description.
    D3D11_SAMPLER_DESC samplerDesc;
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    //Create the texture sampler state.
    result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
    if (FAILED(result))
    {
        return false;
    }

	//Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	D3D11_BUFFER_DESC matrixBufferDesc;
    matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
    matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    matrixBufferDesc.MiscFlags = 0;
    matrixBufferDesc.StructureByteStride = 0;

    //Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
    result = device->CreateBuffer(&matrixBufferDesc, nullptr, &m_matrixBuffer);
    if (FAILED(result))
    {
        return false;
    }

    //Setup the description of the light dynamic constant buffer that is in the pixel shader.
    //Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
    D3D11_BUFFER_DESC lightBufferDesc;
    lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    lightBufferDesc.ByteWidth = sizeof(LightBufferType);
    lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    lightBufferDesc.MiscFlags = 0;
    lightBufferDesc.StructureByteStride = 0;

    //Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
    result = device->CreateBuffer(&lightBufferDesc, nullptr, &m_lightBuffer);
    if (FAILED(result))
    {
        return false;
    }

    return true;
}

void LightShaderClass::ShutdownShader()
{
    //Release the light constant buffer.
    if (m_lightBuffer)
    {
        m_lightBuffer->Release();
        m_lightBuffer = nullptr;
    }

    //Release the matrix constant buffer.
    if (m_matrixBuffer)
    {
        m_matrixBuffer->Release();
        m_matrixBuffer = nullptr;
    }

    //Release the sampler state.
    if (m_sampleState)
    {
        m_sampleState->Release();
        m_sampleState = nullptr;
    }

    //Release the layout.
    if (m_layout)
    {
        m_layout->Release();
        m_layout = nullptr;
    }

    //Release the pixel shader.
    if (m_pixelShader)
    {
        m_pixelShader->Release();
        m_pixelShader = nullptr;
    }

    //Release the vertex shader.
    if (m_vertexShader)
    {
        m_vertexShader->Release();
        m_vertexShader = nullptr;
    }

    return;
}

void LightShaderClass::OutputShaderErrorMessage(ID3DBlob *errorMessage, HWND hwnd, WCHAR *shaderFilename)
{
    //Get a pointer to the error message text buffer.
    char* compileErrors = (char*)(errorMessage->GetBufferPointer());

    //Get the length of the message.
    SIZE_T bufferSize = errorMessage->GetBufferSize();

    //Open a file to write the error message to.
    ofstream fout;
    fout.open("shader-error.txt");

    //Write out the error message.
    for (unsigned long i = 0; i < bufferSize; i++)
    {
        fout << compileErrors[i];
    }

    //Close the file.
    fout.close();

    //Release the error message.
    errorMessage->Release();
    errorMessage = nullptr;

    //Pop a message up on the screen to notify the user to check the text file for compile errors.
    MessageBoxW(hwnd, L"Error compiling shader. Check shader-error.txt for message.", shaderFilename, MB_OK);

    return;
}

bool
LightShaderClass::SetShaderParameters(ID3D11DeviceContext *deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
        XMMATRIX projectionMatrix, ID3D11ShaderResourceView *texture, XMFLOAT3 lightDirection, XMFLOAT4 diffuseColor)
{
    HRESULT result;

    //Transpose the matrices to prepare them for the shader.
    worldMatrix = XMMatrixTranspose(worldMatrix);
    viewMatrix = XMMatrixTranspose(viewMatrix);
    projectionMatrix = XMMatrixTranspose(projectionMatrix);

    //Lock the constant buffer so it can be written to.
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result))
    {
        return false;
    }

    //Get a pointer to the data in the constant buffer.
    MatrixBufferType* dataPtr = (MatrixBufferType*)mappedResource.pData;

    //Copy the matrices into the constant buffer.
    dataPtr->world = worldMatrix;
    dataPtr->view = viewMatrix;
    dataPtr->projection = projectionMatrix;

    //Unlock the constant buffer.
    deviceContext->Unmap(m_matrixBuffer, 0);

    //Set the position of the constant buffer in the vertex shader.
    unsigned int bufferNumber = 0;

    //Now set the constant buffer in the vertex shader with the updated values.
    deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

    //Set shader texture resource in the pixel shader.
    deviceContext->PSSetShaderResources(0, 1, &texture);

    //Lock the light constant buffer so it can be written to.
    result = deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result))
    {
        return false;
    }

    //Get a pointer to the data in the constant buffer.
    LightBufferType* dataPtr2 = (LightBufferType*) mappedResource.pData;

    //Copy the lighting variables into the constant buffer.
    dataPtr2->diffuseColor = diffuseColor;
    dataPtr2->lightDirection = lightDirection;
    dataPtr2->padding = 0.0f;

    //Unlock the constant buffer.
    deviceContext->Unmap(m_lightBuffer, 0);

    //Set the position of the light constant buffer in the pixel shader.
    bufferNumber = 0;

    //Finally set the light constant buffer in the pixel shader with the updated values.
    deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);

    return true;
}

void LightShaderClass::RenderShader(ID3D11DeviceContext *deviceContext, int indexCount)
{
    //Set the vertex input layout.
    deviceContext->IASetInputLayout(m_layout);

    //Set the vertex and pixel shaders that will be used to render this triangle.
    deviceContext->VSSetShader(m_vertexShader, nullptr, 0);
    deviceContext->PSSetShader(m_pixelShader, nullptr, 0);

    //Set the sampler state in the pixel shader.
    deviceContext->PSSetSamplers(0, 1, &m_sampleState);

    //Render the triangle.
    deviceContext->DrawIndexed(indexCount, 0,0);

    return;
}
