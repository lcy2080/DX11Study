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
class LightShaderClass;
class LightClass;

class GraphicsClass
{
public:
    GraphicsClass();
    GraphicsClass(const GraphicsClass&) = default;
    ~GraphicsClass() = default;

    bool Initialize(int, int, HWND);
    void Shutdown();
    bool Frame();

private:
    bool Render(float rotation);

private:
    D3DClass* m_Direct3D;
    CameraClass* m_Camera;
    ModelClass* m_Model;

    LightShaderClass* m_LightShader;
    LightClass* m_Light;
};


#endif //STEP0_GRAPHICSCLASS_H
