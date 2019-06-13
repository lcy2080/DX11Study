//
// Created by LeeChangYoung on 2019-05-25.
//

#ifndef STEP0_GRAPHICSCLASS_H
#define STEP0_GRAPHICSCLASS_H

//#include <windows.h>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENAGLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


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
    bool Render();

private:
    D3DClass* m_D3D;
};


#endif //STEP0_GRAPHICSCLASS_H
