#pragma once

#include "DirectX/DirectXBase.h"

namespace ChangeTileCppCx
{

ref class DirectXLib : public DirectXBase
{
public:
	DirectXLib(void);

	virtual void CreateDeviceIndependentResources() override;
	virtual void CreateDeviceResources() override;
	virtual void Render() override;

	void MakeTile(Platform::String^ path, Platform::String^ text );
	delegate void MakeTitleCompletedHandler();
	event MakeTitleCompletedHandler^ MakeTitleCompleted;

	property Windows::UI::Xaml::Controls::Image^ image1;
	property Platform::String^ _path;

private:
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_solidBrush;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_textFormat;
	Microsoft::WRL::ComPtr<IDWriteTextLayout> m_textLayout;
};

}

