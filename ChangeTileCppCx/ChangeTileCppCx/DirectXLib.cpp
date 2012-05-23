#include "pch.h"
#include "DirectXLib.h"
#include "ppltasks.h"

using namespace ChangeTileCppCx;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::UI::Xaml::Navigation;

using namespace Concurrency;
using namespace Windows::Storage;

DirectXLib::DirectXLib(void)
{
}

void DirectXLib::CreateDeviceIndependentResources()
{
    DirectXBase::CreateDeviceIndependentResources();

    // Create a DirectWrite text format object.
    DX::ThrowIfFailed(
        m_dwriteFactory->CreateTextFormat(
            L"'‚l‚r –¾’©",
            NULL,
            DWRITE_FONT_WEIGHT_REGULAR,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            64.0f,
            L"", // Locale
            &m_textFormat
            )
        );

    // Center the text horizontally and vertically.
    m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
}
void DirectXLib::CreateDeviceResources()
{
    DirectXBase::CreateDeviceResources();

    DX::ThrowIfFailed(
        m_d2dContext->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::MidnightBlue),
            &m_solidBrush
            )
        );
}

void DirectXLib::Render()
{
}

void DirectXLib::MakeTile(Platform::String^ path, Platform::String^ text )
{
	IWICStream *pStream = NULL;
    IWICBitmapEncoder *pEncoder = NULL;
    IWICBitmapFrameEncode *pFrameEncode = NULL;
    static const UINT sc_bitmapWidth = 150;
    static const UINT sc_bitmapHeight = 150;
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
		static const WCHAR sc_fontName[] = L"HGP–¾’©E";
        static const FLOAT sc_fontSize = 50;

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

		static const WCHAR sc_helloWorld[] = L"Hello, World!";
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
		/*
StorageFolder storageFolder = KnownFolders.DocumentsLibrary;
StorageFile sampleFile = await storageFolder.CreateFileAsync("sample.txt");		
		*/
		
//	Windows::Storage::StorageFolder^ folder = 
//		Windows::Storage::KnownFolders::DocumentsLibrary;
	auto folder = 
		Windows::Storage::ApplicationData::Current->LocalFolder;
	Platform::String^ fname = path;
	const wchar_t *filename = fname->Data();
	unsigned char buff[100000];

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

	static Platform::Array<unsigned char>^ buffer = ref new Platform::Array<unsigned char>(sizeof(buff));
	for( int i=0; i<sizeof(buff); i++ ) {
		buffer[i] = buff[i];
	}

	this->_path = path;
	task<StorageFile^>(folder->CreateFileAsync(
		fname, CreationCollisionOption::ReplaceExisting)).then([this](StorageFile^ file)
	{
		task<void>( Windows::Storage::FileIO::WriteBytesAsync( file, buffer )).then([this]() {

			Uri^ uri = ref new Uri( "ms-appdata:///local/" + this->_path );
			BitmapImage ^bi = ref new BitmapImage(uri);
			this->image1->Source = bi ;

		});
	});
}

