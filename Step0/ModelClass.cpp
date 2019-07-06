//
// Created by lcy20 on 2019-06-20.
//

#include "ModelClass.h"
#include <cstdio>


ModelClass::ModelClass()
    : m_vertexBuffer(nullptr)
    , m_indexBuffer(nullptr)
    , m_Texture(nullptr)
{
}

bool ModelClass::Initialize(ID3D11Device *device, ID3D11DeviceContext *deviceContext, char *textureFileName)
{
    bool result;
    //initialize vertex/index buffer
    result = InitializeBuffers(device);
    if (!result)
    {
        return false;
    }

    //Load the texture for this model.
    result = LoadTexture(device, deviceContext, textureFileName);
    if (!result)
    {
        return false;
    }

    return true;
}

void ModelClass::Shutdown()
{
    //Release the model texture.
    ReleaseTexture();

    //release vertex/indexbuffer
    ShutdownBuffers();

    return;
}

void ModelClass::Render(ID3D11DeviceContext *deviceContext)
{
    RenderBuffers(deviceContext);
}

int ModelClass::GetIndexCount()
{
    return m_indexCount;
}

ID3D11ShaderResourceView *ModelClass::GetTexture()
{
    return m_Texture->GetTexture();
}

bool ModelClass::InitializeBuffers(ID3D11Device *device)
{
    //set vertex array length
    m_vertexCount = 3;

    //set index array length
    m_indexCount = 3;

    //make vertex array
    VertexType* vertices = new VertexType[m_vertexCount];
    if (!vertices)
    {
        return false;
    }

    //make index array
    unsigned long* indices = new unsigned long[m_indexCount];
    if (!indices)
    {
        return false;
    }

    //set data on vertex array
    //bottom left
    vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);
    vertices[0].texture = XMFLOAT2(0.f, 1.f);
    vertices[0].normal = XMFLOAT3(0.f, 0.f, -1.f);
    //tob middle
    vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);
    vertices[1].texture = XMFLOAT2(0.5f, 0.f);
    vertices[1].normal = XMFLOAT3(0.f, 0.f, -1.f);
    //bottom right
    vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);
    vertices[2].texture = XMFLOAT2(1.f, 1.f);
    vertices[2].normal = XMFLOAT3(0.f, 0.f, -1.f);
    //set data on index array
    indices[0] = 0; //bottom left
    indices[1] = 1; //top middle
    indices[2] = 2; //bottom right

    //setup static vertex buffer structure
    D3D11_BUFFER_DESC vertexBufferDesc;
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType)* m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    //give vertex data pointer to subresource struct
    D3D11_SUBRESOURCE_DATA vertexData;
    vertexData.pSysMem = vertices;
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    //make vertex buffer
    if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer)))
    {
        return false;
    }

    //set static index buffer struct
    D3D11_BUFFER_DESC indexBufferDesc;
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
    indexBufferDesc.StructureByteStride = 0;

    //make subresource struct pointing index data.
    D3D11_SUBRESOURCE_DATA indexData;
    indexData.pSysMem = indices;
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    //make index buffer;
    if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer)))
    {
        return false;
    }

    //release vertex/index buffer
    delete[] vertices;
    vertices = nullptr;

    delete[] indices;
    indices = nullptr;

    return true;
}

bool ModelClass::LoadTexture(ID3D11Device *device, ID3D11DeviceContext *deviceContext, char *textureFileName)
{
    bool result;

    //Create the texture object.
    m_Texture = new TextureClass;
    if(!m_Texture)
    {
        return false;
    }

    //Initialize the texture object.
    result = m_Texture->Initialize(device, deviceContext, textureFileName);
    if (!result)
    {
        return false;
    }

    return true;
}

void ModelClass::ReleaseTexture()
{
    //Release the texture object.
    if (m_Texture)
    {
        m_Texture->Shutdown();
        delete m_Texture;
        m_Texture = nullptr;
    }
}

void ModelClass::RenderBuffers(ID3D11DeviceContext *deviceContext)
{
    //set vertex buffer's unit and offset
    unsigned int stride = sizeof(VertexType);
    unsigned int offset = 0;

    //set vertex buffer active on input assembler for rendering.
    deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
    //set index buffer active on input assembler for rendering.
    deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    //set vertex default drawing method.
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void ModelClass::ShutdownBuffers()
{
    //release index buffer
    if (m_indexBuffer)
    {
        m_indexBuffer->Release();
        m_indexBuffer = nullptr;
    }

    //release vertex buffer
    if (m_vertexBuffer)
    {
        m_vertexBuffer->Release();
        m_vertexBuffer = nullptr;
    }
}



