using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
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

namespace YogaApp.Views
{
    /// <summary>
    /// Interaction logic for VideoIntro.xaml
    /// </summary>
    public partial class VideoIntro : Page
    {
        private string name;
        //private string YoutubeID;

        public VideoIntro(string value)
        {
            InitializeComponent();
            name = value;
            Intro.Text = getIntro(name);
            Steps.Text = getSteps(name);
            Tips.Text = getTips(name);
        }

        private string getIntro(string name)
        {
            return ExcelReader.getExcelFile("PoseData.xlsx", name + "Intro");
        }

        private string getSteps(string name)
        {
            return ExcelReader.getExcelFile("PoseData.xlsx", name + "Steps");
        }

        private string getTips(string name)
        {
            return ExcelReader.getExcelFile("PoseData.xlsx", name + "Tips");
        }

        //private void Get_YoutubeID(string name)
        //{
        //    YoutubeID = ExcelReader.getExcelFile("PoseData.xlsx", name + "OnlineID");
        //    var temp = youtubeId;
        //}

        private void VideoPlay_Click(object sender, RoutedEventArgs e)
        {
            //Navigation.Navigation.Navigate(new Uri("Views/Video.xaml", UriKind.RelativeOrAbsolute));
            Navigation.Navigation.Navigate(new Video(name));
        }

        private void Kinect_Click(object sender, RoutedEventArgs e)
        {
            //string path = Directory.GetParent(Directory.GetCurrentDirectory()).Parent.FullName;

            string path =
                Directory.GetParent(Directory.GetParent(Directory.GetCurrentDirectory()).Parent.FullName).FullName;

            path += "\\..\\..\\KinectExplorer-D2D\\Debug\\KinectExplorer-D2D.exe";
            Console.WriteLine("saurabh" + path);
            Process.Start(path);
        }


    }
}
