//
// Created by LeeChangYoung on 2019-06-04.
//

#include "d3dclass.h"

D3DClass::D3DClass()
    : m_swapChain(nullptr)
    , m_device(nullptr)
    , m_deviceContext(nullptr)
    , m_renderTargetView(nullptr)
    , m_depthStencilBuffer(nullptr)
    , m_depthStencilState(nullptr)
    , m_depthStencilView(nullptr)
    , m_rasterState(nullptr)
{
}

D3DClass::D3DClass(const D3DClass &)
{

}

D3DClass::~D3DClass()
{

}

bool D3DClass::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
    HRESULT result;
    IDXGIFactory* factory;
    IDXGIAdapter* adapter;
    IDXGIOutput* adapterOutput;
    unsigned int numModes, i, numerator, denominator, stringLength;
    DXGI_MODE_DESC* displayModeList;
    DXGI_ADAPTER_DESC adapterDesc;
    int error;
    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    D3D_FEATURE_LEVEL featureLevel;
    ID3D11Texture2D* backBufferPtr;
    D3D11_TEXTURE2D_DESC depthBufferDesc;
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    D3D11_RASTERIZER_DESC rasterDesc;
    D3D11_VIEWPORT viewport;
    float fieldOfView, screenAspect;

    //store v sync setup
    m_vsync_enabled = vsync;

    //make Direct X graphic interface factory
    result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
    if (FAILED(result))
    {
        return false;
    }

    //through factory object, make adapter about first graphic card interface.
    result = factory->EnumAdapters(0, &adapter);
    if (FAILED(result))
    {
        return false;
    }

    //list first adapter about monitor(output)
    result = adapter->EnumOutputs(0, &adapterOutput);
    if (FAILED(result))
    {
        return false;
    }

    //DXGI_FORMAT_R8G8B8A8_UNORM format : find fit on this format.
    result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
    if (FAILED(result))
    {
        return false;
    }

    //make all available monitor and graphic card combination.
    displayModeList = new DXGI_MODE_DESC[numModes];
    if (!displayModeList)
    {
        return false;
    }

    //fill the list struct about display mode.
    result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
    if (FAILED(result))
    {
        return false;
    }

    for (i = 0; i < numModes; i++)
    {
        if (displayModeList[i].Width == (unsigned int)screenWidth)
        {
            if (displayModeList[i].Height == (unsigned int)screenHeight)
            {
                numerator = displayModeList[i].RefreshRate.Numerator;
                denominator = displayModeList[i].RefreshRate.Denominator;
            }
        }
    }

    //get graphic cards Description
    result = adapter->GetDesc(&adapterDesc);
    if (FAILED(result))
    {
        return false;
    }

    //store current graphic card storage, MB as unit size
    m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

    //store graphic card name as char type.
    error = wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128);
    if (error != 0)
    {
        return false;
    }

    ///////////////////////////////
    // Release Useless Resources //
    ///////////////////////////////

    //release display mode list
    delete [] displayModeList;
    displayModeList = nullptr;

    //release output adepter.
    adapterOutput->Release();
    adapterOutput = nullptr;

    //release adapter.
    adapter->Release();
    adapter = nullptr;

    //release factory object
    factory->Release();
    factory = nullptr;

    ///////////////////////////
    // Initialize BackBuffer //
    ///////////////////////////

    //Initialize Swap chain description
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

    //use only one back buffer.
    swapChainDesc.BufferCount = 1;

    //Set Width and height of back buffer
    swapChainDesc.BufferDesc.Width = screenWidth;
    swapChainDesc.BufferDesc.Height = screenHeight;

    //ordinary 32bit surface as back buffer.
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    //setup back buffer refresh rate
    if(m_vsync_enabled)
    {
        swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
        swapChainDesc.BufferDesc.RefreshRate.Denominator= denominator;
    }
    else
    {
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
        swapChainDesc.BufferDesc.RefreshRate.Denominator= 1;
    }

    //Setup back buffer's usage
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

    //Setup Windows handle for rendering
    swapChainDesc.OutputWindow = hwnd;

    //turn off multisampling
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;

    swapChainDesc.Windowed = !fullscreen;

    //set scanline align and sanlining as unspecified.
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    //discard back buffer content after output
    swapChainDesc.SwapEffect=DXGI_SWAP_EFFECT_DISCARD;

    //do not use additional option flag
    swapChainDesc.Flags = 0;

    //set feature level as DX11
    featureLevel = D3D_FEATURE_LEVEL_11_0;

    //create swap chain, direct3d device. direct3d device context
    result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &featureLevel, 1,
            D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, nullptr, &m_deviceContext);
    if (FAILED(result))
    {
        return false;
    }

    //get back buffer's pointer.
    result = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
    if (FAILED(result))
    {
        return false;
    }

    //create render target view from backbuffer's pointer.
    result = m_device->CreateRenderTargetView(backBufferPtr,nullptr, &m_renderTargetView);
    if (FAILED(result))
    {
        return false;
    }

    //release backbuffer pointer.
    backBufferPtr->Release();
    backBufferPtr = nullptr;

    //////////////////
    // depth buffer //
    //////////////////
    /** Initialize depth buffer description **/
    ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

    /** implement depth buffer description **/
    depthBufferDesc.Width = screenWidth;
    depthBufferDesc.Height = screenHeight;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc. Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;

    /** create depth buffer from description **/
    result = m_device->CreateTexture2D(&depthBufferDesc, nullptr, &m_depthStencilBuffer);
    if(FAILED(result))
    {
        return false;
    }

    ////////////////////
    // stencil buffer //
    ////////////////////
    /** initialize stencil description **/
    ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

    /** implement stencil description **/
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;

    /** stencil operations if pixel is front-facing **/
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp =D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    /** create depth-stencil state **/
    result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
    if(FAILED(result))
    {
        return false;
    }

    /** set depth-stencil state **/
    m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

    /** initialize depth-stencil view description **/
    ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

    /** implement depth-stencil view **/
    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    /** create depth-stencil view **/
    result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
    if (FAILED(result))
    {
        return false;
    }

    /** bind rendertarget view, depth-stencil buffer each output pipeline **/
    m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

    ////////////////
    // rasterizer //
    ////////////////
    /** implement rasterizer that select how to draw each shape **/
    rasterDesc.AntialiasedLineEnable = false;
    rasterDesc.CullMode = D3D11_CULL_BACK;
    rasterDesc.DepthBias = 0;
    rasterDesc.DepthBiasClamp = 0.0f;
    rasterDesc.DepthClipEnable = true;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise = false;
    rasterDesc.MultisampleEnable = false;
    rasterDesc.ScissorEnable = false;
    rasterDesc.SlopeScaledDepthBias = 0.0f;

    /** create rasterizer **/
    result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
    if(FAILED(result))
    {
        return false;
    }

    /** set raterizer state **/
    m_deviceContext->RSSetState(m_rasterState);

    //////////////
    // Viewport //
    //////////////
    /** implement viewport for rendering **/
    viewport.Width = (float)screenWidth;
    viewport.Height = (float)screenHeight;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

    /** create viewport **/
    m_deviceContext->RSSetViewports(1, &viewport);

    ///////////////////////
    // Projection Matrix //
    ///////////////////////
    fieldOfView = (float)DirectX::XM_PI / 4.0f;
    screenAspect = (float)screenWidth / (float)screenHeight;

    /** create Projection Matrix **/
    m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);

    /** initialize world matrix **/
    m_worldMatrix = DirectX::XMMatrixIdentity();

    /** create ortho matrix **/
    m_orthoMatrix = DirectX::XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);

    return true;
}

void D3DClass::Shutdown()
{
    if (m_swapChain)
    {
        m_swapChain->SetFullscreenState(false, nullptr);
    }

    if (m_rasterState)
    {
        m_rasterState->Release();
        m_rasterState = nullptr;
    }

    if (m_depthStencilView)
    {
        m_depthStencilView->Release();
        m_depthStencilView = nullptr;
    }

    if (m_depthStencilState)
    {
        m_depthStencilState->Release();
        m_depthStencilState = nullptr;
    }

    if (m_depthStencilBuffer)
    {
        m_depthStencilBuffer->Release();
        m_depthStencilBuffer = nullptr;
    }

    if (m_renderTargetView)
    {
        m_renderTargetView->Release();
        m_renderTargetView = nullptr;
    }

    if (m_deviceContext)
    {
        m_deviceContext->Release();
        m_deviceContext = nullptr;
    }

    if (m_device)
    {
        m_device->Release();
        m_device = nullptr;
    }

    if (m_swapChain)
    {
        m_swapChain->Release();
        m_swapChain = nullptr;
    }

    return;
}

void D3DClass::BeginScene(float red, float green, float blue, float alpha)
{
    float color[4];

    color[0] = red;
    color[1] = green;
    color[2] = blue;
    color[3] = alpha;

    /** erase back buffer **/
    m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);

    /** erase depth buffer **/
    m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f ,0);

    return;
}


void D3DClass::EndScene()
{
    /** after complete rendering, swap backbuffer **/
    if (m_vsync_enabled)
    {
        /** fix refresh rate **/
        m_swapChain->Present(1, 0);
    }
    else
    {
        m_swapChain->Present(0, 0);
    }

    return;
}

ID3D11Device* D3DClass::GetDevice()
{
    return m_device;
}

ID3D11DeviceContext* D3DClass::GetDeviceContext()
{
    return m_deviceContext;
}


void D3DClass::GetWorldMatrix(DirectX::XMMATRIX& worldMatrix)
{
    worldMatrix = m_worldMatrix;
    return;
}

void D3DClass::GetProjectionMatrix(DirectX::XMMATRIX& projectionMatrix)
{
    projectionMatrix = m_projectionMatrix;
    return;
}

void D3DClass::GetOrthoMatrix(DirectX::XMMATRIX& orthoMatrix)
{
    orthoMatrix = m_orthoMatrix;
    return;
}

void D3DClass::GetVideoCardInfo(char* cardName, int& memory)
{
    strcpy_s(cardName, 128, m_videoCardDescription);
    memory = m_videoCardMemory;
    return;
}

