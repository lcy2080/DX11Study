//
// Created by LeeChangYoung on 2019-05-25.
//

#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
    : m_D3D(nullptr)
{
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
    bool result;

    // Create D3D Object.
    m_D3D = new D3DClass;
    if (!m_D3D)
    {
        return false;
    }

    // Initialize D3D Object
    result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENAGLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
    if (!result)
    {
        MessageBoxW(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
        return false;
    }

    return true;
}

void GraphicsClass::Shutdown()
{
    //return D3D Object
    if(m_D3D)
    {
        m_D3D->Shutdown();
        delete m_D3D;
        m_D3D = nullptr;
    }
    return;
}

bool GraphicsClass::Frame()
{
    bool result;

    //Execute graphic rendering
    result = Render();
    if (!result)
    {
        return false;
    }

    return true;
}

bool GraphicsClass::Render()
{
    //erase buffer before draw scene
    m_D3D->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);

    //draw scene in the buffer
    m_D3D->EndScene();

    return true;
}