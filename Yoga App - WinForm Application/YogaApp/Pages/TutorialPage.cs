using System;
using System.Diagnostics;
using System.Drawing;
using System.Windows.Forms;
using System.IO;
using YogaApp.Utils;


namespace YogaApp
{
    public partial class TutorialPage : Form
    {
        CategoryPage categoryPage;
        CategoryList categoryList;
        public string exchangeName = null;
        public string videoPath = null;
        public string videoDescription = null;


        public TutorialPage(CategoryPage cP)
        {
            InitializeComponent();
            categoryPage = cP;
        }

        public TutorialPage(CategoryList cL)
        {
            InitializeComponent();
            categoryList = cL;

            // add video local path
            try
            {
                exchangeName = categoryList.getName().Name;
            }
            catch (NullReferenceException e)
            {
                Console.WriteLine("Exception caught: {0}", e);
            }
            
        }

        private void playVideobutton_Click(object sender, EventArgs e)
        {
            exchangeName = categoryList.getName().Name;
            videoPath = compareVideoPath(exchangeName);
            string path = Directory.GetParent(Directory.GetCurrentDirectory()).Parent.FullName;
            path += videoPath;
            axWindowsMediaPlayer1.URL = path;

            // add other post here
        }


        private void backPreviousbutton_Click(object sender, EventArgs e)
        {
            this.Hide();
            categoryList.Location = new Point((Screen.PrimaryScreen.WorkingArea.Width - categoryList.Width) / 2,
                         (Screen.PrimaryScreen.WorkingArea.Height - categoryList.Height) / 2);
            categoryList.Show();
            axWindowsMediaPlayer1.close();
            foreach (var process in Process.GetProcessesByName("SkeletonBasics-D2D"))
            {
                Console.WriteLine(process.ToString());
                process.Kill();
            }
        }

        private void tryPoseButton_Click(object sender, EventArgs e)
        {
            string path = Directory.GetParent(Directory.GetCurrentDirectory()).Parent.FullName;
            path += "\\..\\..\\YogaApp-Kinect-cpp\\SkeletonBasics-D2D\\Debug\\SkeletonBasics-D2D.exe";
            Process.Start(path);
        }

        private void richTextBox1_TextChanged(object sender, EventArgs e)
        {
           
            exchangeName = categoryList.getName().Name;
            videoDescription = compareVideoDescription(exchangeName);
            richTextBox1.Text = videoDescription;
        }

        public string compareVideoPath(string ExchangeName)
        {
            return ExcelReader.getExcelFile("PoseData.xlsx", ExchangeName + "VideoPath");
        }

        public string compareVideoDescription(string ExchangeName)
        {
            return ExcelReader.getExcelFile("PoseData.xlsx", ExchangeName + "VideoDescription");
        }
    }
}
