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
    public partial class CategoryList : Form
    {
        TutorialPage tutorialPage;
        CategoryPage categoryPage;
        public CategoryList(CategoryPage cP)
        {
            InitializeComponent();
            categoryPage = cP;
            tutorialPage = new TutorialPage(this);
        }

        private void navigateToTutorialPage()
        {
            this.Hide();
            tutorialPage.Location = new Point((Screen.PrimaryScreen.WorkingArea.Width - tutorialPage.Width) / 2,
                          (Screen.PrimaryScreen.WorkingArea.Height - tutorialPage.Height) / 2);
            tutorialPage.Show();
            
        }

        private void button1_Click(object sender, EventArgs e)
        {
            navigateToTutorialPage();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            navigateToTutorialPage();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            categoryPage.Location = new Point((Screen.PrimaryScreen.WorkingArea.Width - categoryPage.Width) / 2,
                          (Screen.PrimaryScreen.WorkingArea.Height - categoryPage.Height) / 2);
            this.Hide();
            categoryPage.Show();
        }

        private void CategoryList_Load(object sender, EventArgs e)
        {

        }
    }
}
