//
// Created by LeeChangYoung on 2019-05-25.
//

#ifndef STEP0_INPUTCLASS_H
#define STEP0_INPUTCLASS_H


class InputClass
{
public:
    InputClass() = default;
    InputClass(const InputClass&) = default;
    ~InputClass() = default;

    void Initialize();

    void KeyDown(unsigned int);
    void KeyUp(unsigned int);

    bool IsKeyDown(unsigned int);

private:
    bool m_keys[256];
};


#endif //STEP0_INPUTCLASS_H
