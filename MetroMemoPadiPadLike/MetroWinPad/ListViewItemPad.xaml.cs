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
	public sealed partial class ListViewItemPad : ListViewItem
	{
		public ListViewItemPad()
		{
			this.InitializeComponent();
		}
		private DateTime _date;
		private string _title;

		public DateTime Date
		{
			get { return _date; }
			set
			{
				_date = value;
				this.textDate.Text = _date.ToString();
			}
		}
		public string Title
		{
			get { return _title; }
			set
			{
				_title = value;
				this.textTitle.Text = _title;
			}
		}
		public string Text { get; set; }
	}
}
