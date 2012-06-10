using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net.Http;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

namespace MetroWinPad
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        public MainPage()
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
        private void Page_Loaded_1(object sender, RoutedEventArgs e)
        {
            for (int i = 0; i < 20; i++)
            {
                ListViewItemPad item = new ListViewItemPad();
                item.Title = string.Format("title {0}", i);
                item.Date = DateTime.Now;
                item.Text = makeText(item.Title);
                listView1.Items.Add(item);
            }

            Window.Current.SizeChanged += Current_SizeChanged;
            popup1.Button1Click += popup1_Click_1;
            popup1.Visibility = Visibility.Collapsed;
            textBoxAni.Visibility = Visibility.Collapsed;
        }

        string makeText(string title)
        {
            string t = title + "\r\n\r\n";
            for (int i = 0; i < 30; i++)
            {
                t += string.Format("line-{0}", i) + "\r\n";
            }
            return t;
        }

        private void listView1_SelectionChanged_1(object sender, SelectionChangedEventArgs e)
        {
            var item = listView1.SelectedItem as ListViewItemPad;
            textBox1.Text = item.Text;
        }

        private void Current_SizeChanged(object sender, Windows.UI.Core.WindowSizeChangedEventArgs e)
        {
            var state = Windows.UI.ViewManagement.ApplicationView.Value;
            VisualStateManager.GoToState(this, state.ToString(), false);
            App.DebugLog("Current_SizeChanged " + state.ToString());
        }

        private async void DebugLog( string text ) 
        {
            string url = "http://localhost:8083/metro/method";
            HttpClient client = new HttpClient();
            HttpResponseMessage res = await client.GetAsync(url + "/" + text);
            // string response = await res.Content.ReadAsStringAsync();
        }

        private void buttonNew_Click_1(object sender, RoutedEventArgs e)
        {
            App.DebugLog("buttonNew_Click_1 " );


        }

        private void buttonPrev_Click_1(object sender, RoutedEventArgs e)
        {
            int index = listView1.SelectedIndex;
            index--;
            if (0 < index)
            {
                textBoxAni.Text = ((ListViewItemPad)listView1.Items[index]).Text;

                var sb = this.Resources["StoryboardPrev"] as Windows.UI.Xaml.Media.Animation.Storyboard;
                sb.Begin();

                listView1.SelectedIndex = index;
            }
        }

        private void buttonNext_Click_1(object sender, RoutedEventArgs e)
        {
            int index = listView1.SelectedIndex;
            index++;
            if (index < listView1.Items.Count)
            {
                textBoxAni.Text = textBox1.Text;
                var sb = this.Resources["StoryboardNext"] as Windows.UI.Xaml.Media.Animation.Storyboard;
                sb.Begin();
                listView1.SelectedIndex = index;
            }
        }

        private void buttonTrush_Click_1(object sender, RoutedEventArgs e)
        {
            popup1.Visibility = Visibility.Visible;
        }

        private void buttonContract_Click_1(object sender, RoutedEventArgs e)
        {

        }

        private void popup1_Click_1(object sender, RoutedEventArgs e)
        {
            int index = listView1.SelectedIndex;
            if (0 < index)
            {
                textBoxAni.Text = textBox1.Text;
                var sb = this.Resources["StoryboardTrush"] as Windows.UI.Xaml.Media.Animation.Storyboard;
                sb.Begin();
                listView1.Items.RemoveAt(index);
            }
            popup1.Visibility = Visibility.Collapsed;
        }

        private void MainPage_PointerReleased_1(object sender, PointerRoutedEventArgs e)
        {
            popup1.Visibility = Visibility.Collapsed;
        }
    }
}
