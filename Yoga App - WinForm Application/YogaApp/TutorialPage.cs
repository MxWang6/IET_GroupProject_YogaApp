using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

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
            string path = "https://www.youtube.com/v/rQaENEaAHqc?autoplay=1";
            axShockwaveFlash1.LoadMovie(0, path);
        }
      

        private void backPreviousbutton_Click(object sender, EventArgs e)
        {
            this.Hide();
            categoryList.Location = new Point((Screen.PrimaryScreen.WorkingArea.Width - categoryList.Width) / 2,
                         (Screen.PrimaryScreen.WorkingArea.Height - categoryList.Height) / 2);
            categoryList.Show();
            
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Process.Start(@"D:\projects\YogaApp-Kinect-cpp\SkeletonBasics-D2D\Debug\SkeletonBasics-D2D.exe");
        }
    }
}
