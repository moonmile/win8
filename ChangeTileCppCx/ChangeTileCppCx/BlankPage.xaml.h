//
//BlankPage.xaml.h
//BlankPage.xaml クラスの宣言。
//

#pragma once

#include "pch.h"
#include "BlankPage.g.h"

namespace ChangeTileCppCx
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class BlankPage sealed
	{
	public:
		BlankPage();

	protected:
		virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
	private:
		void Button_Click_1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Button_Click_2(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
