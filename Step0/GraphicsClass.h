//
// Created by LeeChangYoung on 2019-05-25.
//

#ifndef STEP0_GRAPHICSCLASS_H
#define STEP0_GRAPHICSCLASS_H

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class D3DClass;
class CameraClass;
class ModelClass;
class ColorShaderClass;

class GraphicsClass
{
public:
    GraphicsClass() = default;
    GraphicsClass(const GraphicsClass&) = default;
    ~GraphicsClass() = default;

    bool Initialize(int, int, HWND);
    void Shutdown();
    bool Frame();

private:
    bool Render();

private:
    D3DClass* m_Direct3D = nullptr;
    CameraClass* m_Camera = nullptr;
    ModelClass* m_Model = nullptr;
    ColorShaderClass* m_ColorShader = nullptr;
};


#endif //STEP0_GRAPHICSCLASS_H
