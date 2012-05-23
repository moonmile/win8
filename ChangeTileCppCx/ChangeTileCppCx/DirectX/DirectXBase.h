#pragma once

#include "DirectXHelper.h"

// Helper class that initializes DirectX APIs
ref class DirectXBase abstract
{
public:
    DirectXBase();

    virtual void Initialize(Windows::UI::Core::CoreWindow^ window);
    virtual void CreateDeviceIndependentResources();
    virtual void CreateDeviceResources();
    virtual void CreateWindowSizeDependentResources();
    virtual void UpdateForWindowSizeChange();
    virtual void SetDpi(float dpi);
    virtual void Render() = 0;
    virtual void Present();

protected:

    // Direct2D Objects
    Microsoft::WRL::ComPtr<ID2D1Factory1>          m_d2dFactory;
    Microsoft::WRL::ComPtr<ID2D1Device>            m_d2dDevice;
    Microsoft::WRL::ComPtr<ID2D1DeviceContext>     m_d2dContext;
    Microsoft::WRL::ComPtr<ID2D1Bitmap1>           m_d2dTargetBitmap;

    // DirectWrite & Windows Imaging Component Objects
    Microsoft::WRL::ComPtr<IDWriteFactory1>        m_dwriteFactory;
    Microsoft::WRL::ComPtr<IWICImagingFactory2>    m_wicFactory;

    // Direct3D Objects
    Microsoft::WRL::ComPtr<ID3D11Device1>          m_d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1>   m_d3dContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain1>        m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;

    D3D_FEATURE_LEVEL                              m_featureLevel;
    Windows::Foundation::Size                      m_renderTargetSize;
    Windows::Foundation::Rect                      m_windowBounds;
    Windows::UI::Core::CoreWindow^                 m_window;
    float                                          m_dpi;
};