using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

using Windows.UI.Xaml.Media.Imaging;
using Windows.Storage;
using Windows.Storage.Streams;
using Windows.Graphics.Imaging;
using System.Runtime.InteropServices.WindowsRuntime;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

namespace DynamicMakeWritableBitmap
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class BlankPage : Page
    {
        public BlankPage()
        {
            this.InitializeComponent();
        }

        /// <summary>
        /// Invoked when this page is about to be displayed in a Frame.
        /// </summary>
        /// <param name="e">Event data that describes how this page was reached.  The Parameter
        /// property is typically used to configure the page.</param>
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
        }

		private void Button_Click_1(object sender, RoutedEventArgs e)
		{
			var bi = new BitmapImage(
				new Uri("ms-appx:///Images/Logo2.png"));
			image1.Source = bi;
		}

		private async void Button_Click_2(object sender, RoutedEventArgs e)
		{
			var mem = new InMemoryRandomAccessStream();
			var enc = await BitmapEncoder.CreateAsync(BitmapEncoder.PngEncoderId, mem);

			int width = 140, height = 140;
			int plane = 4;
			byte[] data = new byte[width * height * plane];
			int offset = 0;
			for (int y = 0; y < width; y++)
			{
				for (int x = 0; x < height; x++)
				{
					data[offset + 0] = 0xFF; // R
					data[offset + 1] = 0x00; // G
					data[offset + 2] = 0x00; // B
					data[offset + 3] = 0xFF; // Alpha
					offset += 4;
				}
			}
			enc.SetPixelData(BitmapPixelFormat.Rgba8, BitmapAlphaMode.Straight,
				(uint)width, (uint)height, 96, 96, data);
			await enc.FlushAsync();

			BitmapImage bmp = new BitmapImage();
			bmp.SetSource(mem);
			image1.Source = bmp;
		}

		private async void Button_Click_3(object sender, RoutedEventArgs e)
		{
			var dxbmp = new MakeBitmapLib.DXBitmap();
			dxbmp.Initialize();
			byte[] data = dxbmp.MakeText("あれこれ");

			var mem = new InMemoryRandomAccessStream();
			await mem.WriteAsync(data.AsBuffer());
			// await mem.FlushAsync();
			BitmapImage bmp = new BitmapImage();
			bmp.SetSource(mem);
			image1.Source = bmp;
		}
    }
}
