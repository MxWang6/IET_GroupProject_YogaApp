using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using YogaApp.Utils;
using YoutubePlayerLib.Cef;

namespace YogaApp.Views
{
    /// <summary>
    /// Interaction logic for Video.xaml
    /// </summary>
    public partial class Video : Page
    {
        private string name;
        public Video(string value)
        {
            InitializeComponent();
            name = value;
            SetPlayer(name);
        }

        private void SetPlayer(string name)
        {
            Player.VideoId = GetVideoID(name);
        }

        private string GetVideoID(string name)
        {
            string id = ExcelReader.getExcelFile("PoseData.xlsx", name + "OnlineID");
            return id;
        }
    }
}
