using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Web;
using System.Windows.Forms;

namespace MetroLogServer
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        HttpListener listener;

        private void button1_Click(object sender, EventArgs e)
        {
            string url = "http://*:8083/metro/";
            // 開始
            listener = new HttpListener();
            listener.Prefixes.Add(url);

            listener.Start();
            listBox1.Items.Add("サーバー開始");
            listener.BeginGetContext(ListenerCallback, listener);
        }

        public void ListenerCallback(IAsyncResult result)
        {
            // HttpListener listener = (HttpListener)result.AsyncState;
            HttpListenerContext context;
            try
            {
                context = listener.EndGetContext(result);
            }
            catch
            {
                // stop メソッドで例外が発生するので、対処
                return;
            }
            // var content = listener.GetContext();
            var req = context.Request;
            var url = req.RawUrl;
            var res = context.Response;

            string text = url.Replace("/metro/method/", "");
            text = System.Web.HttpUtility.UrlDecode(text);
            listBox1.Items.Add("受信:" +  text);

            var output = new StreamWriter(res.OutputStream);
            output.WriteLine(string.Format("called {0}", url));
            output.Close();

            // 次の受信の準備
            listener.BeginGetContext(ListenerCallback, listener);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            // 終了
            listener.Stop();
            listBox1.Items.Add("サーバー終了");
        }
    }
}
