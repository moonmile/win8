// WinRTComponent.cpp
#include "pch.h"
#include "WinRTComponent.h"

using namespace MakeBitmapLib;
using namespace Platform;
using namespace Microsoft::WRL;
using namespace Windows::UI::Core;
using namespace Windows::Foundation;
using namespace D2D1;
using namespace Windows::Storage::Streams;

DXBitmap::DXBitmap()
{
}
void DXBitmap::Initialize()
{
	// CreateDeviceIndependentResources
    D2D1_FACTORY_OPTIONS options;
    ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

#if defined(_DEBUG)
     // If the project is in a debug build, enable Direct2D debugging via SDK Layers.
    options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

    DX::ThrowIfFailed(
        D2D1CreateFactory(
            D2D1_FACTORY_TYPE_SINGLE_THREADED, 
            __uuidof(ID2D1Factory1),
            &options, 
            &m_d2dFactory
            )
        );

    DX::ThrowIfFailed(
        DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            &m_dwriteFactory
            )
        );

	DX::ThrowIfFailed(
        CoCreateInstance(
            CLSID_WICImagingFactory,
            nullptr,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&m_wicFactory)
            )
        );


    // This flag adds support for surfaces with a different color channel ordering than the API default.
    // It is recommended usage, and is required for compatibility with Direct2D.
    UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
    // If the project is in a debug build, enable debugging via SDK Layers with this flag.
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    // This array defines the set of DirectX hardware feature levels this app will support.
    // Note the ordering should be preserved.
    // Don't forget to declare your application's minimum required feature level in its
    // description.  All applications are assumed to support 9.1 unless otherwise stated.
    D3D_FEATURE_LEVEL featureLevels[] = 
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1
    };

    // Create the DX11 API device object, and get a corresponding context.
    ComPtr<ID3D11Device> d3dDevice;
    ComPtr<ID3D11DeviceContext> d3dContext;
    DX::ThrowIfFailed(
        D3D11CreateDevice(
            nullptr,                  // specify null to use the default adapter
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,                  // leave as nullptr unless software device
            creationFlags,            // optionally set debug and Direct2D compatibility flags
            featureLevels,            // list of feature levels this app can support
            ARRAYSIZE(featureLevels), // number of entries in above list
            D3D11_SDK_VERSION,        // always set this to D3D11_SDK_VERSION for modern
            &d3dDevice,               // returns the Direct3D device created
            &m_featureLevel,          // returns feature level of device created
            &d3dContext               // returns the device immediate context
            )
        );

    // Get the DirectX11.1 device by QI off the DirectX11 one.
    DX::ThrowIfFailed(
        d3dDevice.As(&m_d3dDevice)
        );

    // And get the corresponding device context in the same way.
    DX::ThrowIfFailed(
        d3dContext.As(&m_d3dContext)
        );

    // Obtain the underlying DXGI device of the Direct3D11.1 device.
    ComPtr<IDXGIDevice> dxgiDevice;
    DX::ThrowIfFailed(
        m_d3dDevice.As(&dxgiDevice)
        );

    // Obtain the Direct2D device for 2D rendering.
    DX::ThrowIfFailed(
        m_d2dFactory->CreateDevice(dxgiDevice.Get(), &m_d2dDevice)
        );

    // And get its corresponding device context object.
    DX::ThrowIfFailed(
        m_d2dDevice->CreateDeviceContext(
            D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
            &m_d2dContext
            )
        );

    // Save the DPI of this display in our class.
    m_d2dContext->SetDpi(m_dpi, m_dpi);

    // Release the swap chain (if it exists) as it will be incompatible with
    // the new device.
    m_swapChain = nullptr;

}
Platform::Array<unsigned char>^ DXBitmap::MakeText( Platform::String^ text )
{
	IWICStream *pStream = NULL;
    IWICBitmapEncoder *pEncoder = NULL;
    IWICBitmapFrameEncode *pFrameEncode = NULL;
    static const UINT sc_bitmapWidth = 140;
    static const UINT sc_bitmapHeight = 140;
    IWICBitmap *pWICBitmap = NULL;
    ID2D1RenderTarget *pRT = NULL;
    IDWriteTextFormat *pTextFormat = NULL;
    ID2D1SolidColorBrush *pBlackBrush = NULL;


	DX::ThrowIfFailed(
		m_wicFactory->CreateBitmap(
            sc_bitmapWidth,
            sc_bitmapHeight,
            GUID_WICPixelFormat32bppBGR,
            WICBitmapCacheOnLoad,
            &pWICBitmap
			));

	DX::ThrowIfFailed(
        m_d2dFactory->CreateWicBitmapRenderTarget(
            pWICBitmap,
            D2D1::RenderTargetProperties(),
            &pRT
            ));

        pRT->BeginDraw();
        pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));
        D2D1_SIZE_F rtSize = pRT->GetSize();

		// static const WCHAR sc_fontName[] = L"Calibri";
		static const WCHAR sc_fontName[] = L"HGP明朝E";
        static const FLOAT sc_fontSize = 32;

        m_dwriteFactory->CreateTextFormat(
            sc_fontName,
            NULL,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            sc_fontSize,
            L"", //locale
            &pTextFormat
            );

        pRT->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Black),
            &pBlackBrush
            );

        pRT->DrawText(
			text->Data(),
			text->Length(),
            pTextFormat,
            D2D1::RectF(0, 0, rtSize.width, rtSize.height),
            pBlackBrush);

		pRT->EndDraw();

    // Save image to file
    DX::ThrowIfFailed(
		m_wicFactory->CreateStream(&pStream));

    WICPixelFormatGUID format = GUID_WICPixelFormatDontCare;
    //    static const WCHAR filename[] = L"output.png";

	unsigned char buff[sc_bitmapWidth*sc_bitmapHeight*4];


//	DX::ThrowIfFailed(
//		pStream->InitializeFromFilename(filename, GENERIC_WRITE));
	DX::ThrowIfFailed(
		pStream->InitializeFromMemory( buff, sizeof(buff)));
    DX::ThrowIfFailed(
		m_wicFactory->CreateEncoder(GUID_ContainerFormatPng, NULL, &pEncoder));
	DX::ThrowIfFailed(
        pEncoder->Initialize(pStream, WICBitmapEncoderNoCache));
    DX::ThrowIfFailed(
        pEncoder->CreateNewFrame(&pFrameEncode, NULL));
	DX::ThrowIfFailed(
		pFrameEncode->Initialize(NULL));
	DX::ThrowIfFailed(
        pFrameEncode->SetSize(sc_bitmapWidth, sc_bitmapHeight));
    DX::ThrowIfFailed(
        pFrameEncode->SetPixelFormat(&format));
    DX::ThrowIfFailed(
        pFrameEncode->WriteSource(pWICBitmap, NULL));
	DX::ThrowIfFailed(
        pFrameEncode->Commit());
	DX::ThrowIfFailed(
        pEncoder->Commit());

	Platform::Array<unsigned char>^ buffer = ref new Platform::Array<unsigned char>(sizeof(buff));
	for( int i=0; i<sizeof(buff); i++ ) {
		buffer[i] = buff[i];
	}
	return buffer;
}
