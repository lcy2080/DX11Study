//
// Created by lcy20 on 2019-06-29.
//

#ifndef STEP0_TEXTURECLASS_H
#define STEP0_TEXTURECLASS_H

#include <d3d11.h>

#include "AlignedAllocationPolicy.h"

class TextureClass : public AlignedAllocationPolicy<16>
{
private:

    struct TargaHeader
    {
        unsigned char data1[12];
        unsigned short width;
        unsigned short height;
        unsigned char bpp;
        unsigned char data2;
    };

public:
    TextureClass();
    TextureClass(const TextureClass& other) = default;
    ~TextureClass() = default;

    bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename);
    void Shutdown();

    ID3D11ShaderResourceView* GetTexture();

private:
    bool LoadTarga(char* filename, int& height, int& width);

private:
    unsigned char* m_targaData;
    ID3D11Texture2D* m_texture;
    ID3D11ShaderResourceView* m_textureView;
};


#endif //STEP0_TEXTURECLASS_H
