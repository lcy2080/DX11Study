//
// Created by LeeChangYoung on 2019-05-24.
// Filename : systemclass.h
//

#ifndef STEP0_SYSTEMCLASS_H
#define STEP0_SYSTEMCLASS_H

///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define WIN32_LEAN_AND_MEAN

//////////////
// INCLUDES //
//////////////
#include <windows.h>
#include <memory>

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "InputClass.h"
#include "GraphicsClass.h"

//////////////////////////////
// CLASS NAME : SYSTEMCLASS //
//////////////////////////////
class SystemClass {
public:
    SystemClass();
    SystemClass(const SystemClass&);
    ~SystemClass();

    bool Initialize();
    void Shutdown();
    void Run();

    LRESULT  CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
    bool Frame();
    void InitializeWindows(int&, int&);
    void ShutdownWindows();

private:
    LPCWSTR m_applicationName;
    HINSTANCE m_hinstance;
    HWND m_hwnd;

    std::unique_ptr<InputClass> m_Input;
    std::unique_ptr<GraphicsClass> m_Graphics;
};

/////////////////////////
// FUNCTION PROTOTYPES //
/////////////////////////
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

/////////////
// GLOBALS //
/////////////
static SystemClass* ApplicationHandle = 0;

#endif //STEP0_SYSTEMCLASS_H
