//
// Created by LeeChangYoung on 2019-05-25.
//

#include "d3dclass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "GraphicsClass.h"
#include "LightShaderClass.h"
#include "LightClass.h"

GraphicsClass::GraphicsClass()
    : m_Direct3D(nullptr)
    , m_Camera(nullptr)
    , m_Model(nullptr)
    , m_LightShader(nullptr)
    , m_Light(nullptr)
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
    // Create D3D Object.
    m_Direct3D = new D3DClass;
    if (!m_Direct3D)
    {
        return false;
    }

    // Initialize D3D Object
    if (!m_Direct3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
    {
        MessageBoxW(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
        return false;
    }

    //generate camera object
    m_Camera = new CameraClass;
    if (!m_Camera)
    {
        return false;
    }

    m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

    //generate model object
    m_Model = new ModelClass;
    if (!m_Model)
    {
        return false;
    }

    //Initialize the model object.
    if (!m_Model->Initialize(m_Direct3D->GetDevice(), m_Direct3D->GetDeviceContext(), "E:/Git/DX11/Step0/rock_02_dif.tga"))
    {
        MessageBoxW(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
        return false;
    }

    // Create the light shader object.
    m_LightShader = new LightShaderClass;
    if (!m_LightShader)
    {
        return false;
    }

    //Initialize the light shader object.
    if (!m_LightShader->Initialize(m_Direct3D->GetDevice(), hwnd))
    {
        MessageBoxW(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
        return false;
    }

    //Create the light object.
    m_Light = new LightClass;
    if (!m_Light)
    {
        return false;
    }

    //Initialize the light object.
    m_Light->SetDiffuseColor(1.f, 0.f, 1.f, 1.f);
    m_Light->SetDirection(0.f, 0.f, 1.f);

    return true;
}

void GraphicsClass::Shutdown()
{
    //Release the light object.
    if (m_Light)
    {
        delete m_Light;
        m_Light = nullptr;
    }

    //Release light shader object
    if (m_LightShader)
    {
        m_LightShader->Shutdown();
        delete m_LightShader;
        m_LightShader = nullptr;
    }

    //return model object
    if (m_Model)
    {
        m_Model->Shutdown();
        delete m_Model;
        m_Model = nullptr;
    }

    //return camera object
    if (m_Camera)
    {
        delete m_Camera;
        m_Camera = nullptr;
    }

    //return D3D Object
    if(m_Direct3D)
    {
        m_Direct3D->Shutdown();
        delete m_Direct3D;
        m_Direct3D = nullptr;
    }
}

bool GraphicsClass::Frame()
{
    static float rotation = 0.f;

    //Update the rotation variable each frame.
    rotation += XM_PI * 0.01f;
    if (rotation > 360.f)
    {
        rotation -= 360.f;
    }

    return Render(rotation);
}

bool GraphicsClass::Render(float rotation)
{
    //erase buffer before draw scene
    m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

    //generate view matrix along camera position
    m_Camera->Render();

    //get view and projection matrix from camera and d3d object
    XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
    m_Direct3D->GetWorldMatrix(worldMatrix);
    m_Camera->GetViewMatrix(viewMatrix);
    m_Direct3D->GetProjectionMatrix(projectionMatrix);

    //Rotate the world matrix by the rotation vlaue so that the triangle will spin.
    worldMatrix = worldMatrix * XMMatrixRotationY(rotation);

    //ready to drawing, ...
    m_Model->Render(m_Direct3D->GetDeviceContext());

    //render the model using the light shader.
    if (!m_LightShader->Render(m_Direct3D->GetDeviceContext(),
            m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(), m_Light->GetDirection(), m_Light->GetDiffuseColor()))
    {
        return false;
    }

    //draw scene in the buffer
    m_Direct3D->EndScene();

    return true;
}

