//
// Created by lcy20 on 2019-06-29.
//

#include <cstdio>
#include "TextureClass.h"

TextureClass::TextureClass()
{
    m_targaData = nullptr;
    m_texture = nullptr;
    m_textureView = nullptr;
}

bool TextureClass::Initialize(ID3D11Device *device, ID3D11DeviceContext *deviceContext, char *filename)
{
    bool result;
    int height, width;
    HRESULT hResult;

    //Load the targa image data into memory.
    result = LoadTarga(filename, height, width);
    if (!result)
    {
        return false;
    }

    //Setup the description of the texture.
    D3D11_TEXTURE2D_DESC textureDesc;
    textureDesc.Height = height;
    textureDesc.Width = width;
    textureDesc.MipLevels = 0;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    textureDesc.CPUAccessFlags = 0;
    textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

    //Create the empty texture.
    hResult = device->CreateTexture2D(&textureDesc, nullptr, &m_texture);
    if(FAILED(hResult))
    {
        return false;
    }

    //Set the row pitch of the targa image data.
    unsigned int rowPitch = (width * 3) * sizeof(unsigned char);

    //Copy the targa image data into the texture.
    deviceContext->UpdateSubresource(m_texture, 0, nullptr, m_targaData, rowPitch, 0);

    //Setup the shader resource view description.
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = textureDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = -1;

    //Create the shader resource view for the texture.
    hResult = device->CreateShaderResourceView(m_texture, &srvDesc, &m_textureView);
    if (FAILED(hResult))
    {
        return false;
    }

    //Generate mipmaps for this texture.
    deviceContext->GenerateMips(m_textureView);

    //Release the targa image data now that the image data has benn loaded into the texture.
    delete [] m_targaData;
    m_targaData = nullptr;

    return true;
}

void TextureClass::Shutdown()
{
    //Release the texture view resource.
    if (m_textureView)
    {
        m_textureView->Release();
        m_textureView = nullptr;
    }

    //Release the texture.
    if (m_texture)
    {
        m_texture->Release();
        m_texture = nullptr;
    }

    //Release the targa data.
    if (m_targaData)
    {
        delete [] m_targaData;
        m_targaData = nullptr;
    }

    return;
}

ID3D11ShaderResourceView *TextureClass::GetTexture()
{
    return m_textureView;
}

bool TextureClass::LoadTarga(char *filename, int &height, int &width)
{
    int error;

    //Open the targa file for reading in binary.
    FILE* filePtr;
    error = fopen_s(&filePtr, filename, "rb");
    if (error != 0)
    {
        return false;
    }

    //Read in the file header.
    TargaHeader targaFileHeader;
    unsigned int count = (unsigned int) fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
    if (count != 1)
    {
        return false;
    }

    //Get the important information from the header.
    int bpp;
    height  = (int) targaFileHeader.height;
    width   = (int) targaFileHeader.width;
    bpp     = (int) targaFileHeader.bpp;

    //Check that it is 32 bit and not 24bit.
	//32 bit has some problem.
    if (bpp != 24)
    {
        return false;
    }

    //Calculate the size of the 32 bit image data.
    int imageSize = width * height * 3;

    //Allocate memory for the targa image data.
    unsigned char* targaImage;
    targaImage = new unsigned char[imageSize];
    if (!targaImage)
    {
        return false;
    }

    //Read in the targa image data.
    count = (unsigned int) fread(targaImage, 1, imageSize, filePtr);
    if(count != imageSize)
    {
        return false;
    }

    //Close the file.
    error = fclose(filePtr);
    if(error != 0)
    {
        return false;
    }

    //Allocate memory for the targa destination data.
    m_targaData = new unsigned char[imageSize];
    if (!m_targaData)
    {
        return false;
    }

    //initialize the index into the targa destination data array.
    int index = 0;

    //initialize the index into the targa image data.
    int k = (width * height * 3) - (width * 3);

    //Now copy the targa image data into the targa destination array in the correct order since the targa format is stored upside down.
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            m_targaData[index + 0] = targaImage[k + 2]; // Red
            m_targaData[index + 1] = targaImage[k + 1]; // Green
            m_targaData[index + 2] = targaImage[k + 0]; // Blue
            //m_targaData[index + 3] = targaImage[k + 3]; // Alpha

            //Increment the indexes into the targa data.
            k += 3;
            index += 3;
        }
        //Set the targa image data index back to the preceding row at the beginning of the column since its reading it in upside down.
        k -= (width * 6);
    }

    //Release the targa image data now that it was copied into the destination array.
    delete [] targaImage;
    targaImage = nullptr;

    return true;
}
