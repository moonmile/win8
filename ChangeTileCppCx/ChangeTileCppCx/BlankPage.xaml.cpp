//
//BlankPage.xaml.cpp
//BlankPage.xaml クラスの実装。
//

#include "pch.h"
#include "BlankPage.xaml.h"
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
using namespace Windows::UI::Xaml::Navigation;

using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Concurrency;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;
using namespace Windows::Data::Xml::Dom;
using namespace Windows::UI::Notifications;


// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

BlankPage::BlankPage()
{
	InitializeComponent();
}

/// <summary>
/// Invoked when this page is about to be displayed in a Frame.
/// </summary>
/// <param name="e">Event data that describes how this page was reached.  The Parameter
/// property is typically used to configure the page.</param>
void BlankPage::OnNavigatedTo(NavigationEventArgs^ e)
{
}


void ChangeTileCppCx::BlankPage::Button_Click_1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	// 参照をクリア
	imageTile->Source = nullptr;

	// 文字を設定してpngファイルを作成
	String^ text = this->textTitle->Text;

	DirectXLib^ lib = ref new DirectXLib();
	lib->CreateDeviceIndependentResources();
	lib->CreateDeviceResources();
	Platform::String ^fname = L"tile001.png";
	
	// Image::Soruce に設定
	// 本当は lambda にしたいところ
	lib->image1 = imageTile;
	lib->MakeTile( fname, text );
}


void ChangeTileCppCx::BlankPage::Button_Click_2(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	// タイルをアップデート
	TileTemplateType tileTemplate = TileTemplateType::TileSquareImage;
	XmlDocument^ doc = TileUpdateManager::GetTemplateContent( tileTemplate );
	String^ xml = doc->GetXml();

	auto images = doc->GetElementsByTagName("image");
	XmlElement^ img = (XmlElement^)images->Item(0);
	img->SetAttribute(L"src","ms-appdata:///local/tile001.png");
	// img->SetAttribute(L"src","ms-appdata:///local/Logo1.png");

	TileNotification^ notify = ref new TileNotification( doc );
	TileUpdateManager::CreateTileUpdaterForApplication()->Update(notify);

}
