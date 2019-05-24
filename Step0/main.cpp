#include <iostream>

#include "Systemclass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
    Systemclass* System;
    bool result;

    System = new Systemclass();
    if (!System)
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