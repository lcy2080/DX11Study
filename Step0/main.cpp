#include <windows.h>
#include <iostream>

#include "SystemClass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
    SystemClass* System;
    bool result;

    System = new SystemClass();
    if (nullptr == System)
    {
        return 0;
    }

    result = System->Initialize();
    if (result)
    {
        System->Run();
    }

    System->Shutdown();
    delete System;
    System = nullptr;

    return 0;
}