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
        //TutorialPage tutorialPage;
        CategoryList categoryList;
        public CategoryPage()
        {
            InitializeComponent();
            //tutorialPage = new TutorialPage(this);
            categoryList = new CategoryList(this);
        }

        private void childButton_Click(object sender, EventArgs e)
        {
            categoryList.Location = new Point((Screen.PrimaryScreen.WorkingArea.Width - categoryList.Width) / 2,
                          (Screen.PrimaryScreen.WorkingArea.Height - categoryList.Height) / 2);
            this.Hide();
            categoryList.Show();

        }

        private void button2_Click(object sender, EventArgs e)
        {
            this.Hide();
            //  categoryPage.Show();
            //add local path to connect kinect.exe file
            Process.Start(@"D:\projects\YogaApp-Kinect-cpp\SkeletonBasics-D2D\Debug\SkeletonBasics-D2D.exe");
        }

        private void button3_Click(object sender, EventArgs e)
        {
            this.Hide();
            categoryList.Location = new Point((Screen.PrimaryScreen.WorkingArea.Width - categoryList.Width) / 2,
                          (Screen.PrimaryScreen.WorkingArea.Height - categoryList.Height) / 2);
            categoryList.Show();
        }
    }
}
