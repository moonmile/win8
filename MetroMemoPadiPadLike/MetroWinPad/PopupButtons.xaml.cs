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

// The User Control item template is documented at http://go.microsoft.com/fwlink/?LinkId=234236

namespace MetroWinPad
{
	public sealed partial class PopupButtons : UserControl
	{
		public PopupButtons()
		{
			this.InitializeComponent();

			button1.Click += button1_Click;
		
		}

		public RoutedEventHandler Button1Click;

		void button1_Click(object sender, RoutedEventArgs e)
		{
			if (Button1Click != null)
			{
				this.Button1Click(sender, e);
			}
		}

		public string Content1
		{
			get { return (string)button1.Content; }
			set { button1.Content = value; }
		}

	}
}
