using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;

namespace YogaApp
{
    public partial class CategoryPage : Form
    {
        TutorialPage tutorialPage;
        public CategoryPage()
        {
            InitializeComponent();
            tutorialPage = new TutorialPage(this);
        }

        private void childButton_Click(object sender, EventArgs e)
        {
            this.Hide(); 
         //   tutorialPage.DesktopLocation = new Point(this.ClientSize.Width / 2, this.ClientSize.Height / 2);
            tutorialPage.Location = new Point((Screen.PrimaryScreen.WorkingArea.Width - tutorialPage.Width) / 2,
                          (Screen.PrimaryScreen.WorkingArea.Height - tutorialPage.Height) / 2);
            tutorialPage.Show();

        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.Hide();
            //  categoryPage.Show();
            //add local path to connect kinect.exe file
            Process.Start(@"D:\projects\YogaApp-Kinect-cpp\SkeletonBasics-D2D\Debug\SkeletonBasics-D2D.exe");
        }

      
    }
}
