#pragma once

namespace DX
{
    inline void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            // Set a breakpoint on this line to catch DirectX API errors
            throw Platform::Exception::CreateException(hr);
        }
    }
}

namespace MakeBitmapLib
{
    public ref class DXBitmap sealed
    {
    public:
        DXBitmap();
		void Initialize();
		Platform::Array<unsigned char>^ MakeText( Platform::String^ text );

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
}