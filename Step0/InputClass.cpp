//
// Created by LeeChangYoung on 2019-05-25.
//

#include "InputClass.h"

void InputClass::Initialize()
{
    for (int i = 0; i < 256; i++)
    {
        m_keys[i] = false;
    }

    return;
}

void InputClass::KeyDown(unsigned int input)
{
    m_keys[input] = true;
}

void InputClass::KeyUp(unsigned int input)
{
    m_keys[input] = false;
}

bool InputClass::IsKeyDown(unsigned int key)
{
    return m_keys[key];
}
