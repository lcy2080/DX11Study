//
// Created by LeeChangYoung on 2019-05-25.
//

#include "d3dclass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "ColorShaderClass.h"
#include "GraphicsClass.h"

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

    if (!m_Model->Initialize(m_Direct3D->GetDevice()))
    {
        MessageBoxW(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
        return false;
    }

    //generate color shader object
    m_ColorShader = new ColorShaderClass;
    if (!m_ColorShader)
    {
        return false;
    }

    if (!m_ColorShader->Initialize(m_Direct3D->GetDevice(), hwnd))
    {
        MessageBoxW(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
        return false;
    }

    return true;
}

void GraphicsClass::Shutdown()
{
    //return color shader object
    if (m_ColorShader)
    {
        m_ColorShader->Shutdown();
        delete m_ColorShader;
        m_ColorShader = nullptr;
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
    return Render();
}

bool GraphicsClass::Render()
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

    //ready to drawing, ...
    m_Model->Render(m_Direct3D->GetDeviceContext());

    //render model
    if (!m_ColorShader->Render(m_Direct3D->GetDeviceContext(),
            m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix))
    {
        return false;
    }

    //draw scene in the buffer
    m_Direct3D->EndScene();

    return true;
}