using System;
using System.Diagnostics;
using System.Drawing;
using System.Windows.Forms;
using System.IO;

namespace YogaApp
{
    public partial class TutorialPage : Form
    {
        CategoryPage categoryPage;
        CategoryList categoryList;
       

        public TutorialPage(CategoryPage cP)
        {
            InitializeComponent();
            categoryPage = cP;
        }

        public TutorialPage(CategoryList cL)
        {
            InitializeComponent();
            categoryList = cL;
        }

        private void playVideobutton_Click(object sender, EventArgs e)
        {
            //  string path = "https://www.youtube.com/v/rQaENEaAHqc?autoplay=1";
            //axShockwaveFlash1.LoadMovie(0, path);

            string path = "D:\\Project\\YogaVideos\\TreePose.mp4";
            axWindowsMediaPlayer1.URL = @path;
           
        }


        private void backPreviousbutton_Click(object sender, EventArgs e)
        {
            this.Hide();
            categoryList.Location = new Point((Screen.PrimaryScreen.WorkingArea.Width - categoryList.Width) / 2,
                         (Screen.PrimaryScreen.WorkingArea.Height - categoryList.Height) / 2);
            categoryList.Show();
            foreach (var process in Process.GetProcessesByName("SkeletonBasics-D2D"))
            {
                Console.WriteLine(process.ToString());
                process.Kill();
            }
        }

        private void tryPoseButton_Click(object sender, EventArgs e)
        {
            string path = Directory.GetParent(Directory.GetCurrentDirectory()).Parent.FullName;
              path = "\\..\\..\\YogaApp-Kinect-cpp\\SkeletonBasics-D2D\\Debug\\SkeletonBasics-D2D.exe";
            Process.Start(path);
        }
    }
}
