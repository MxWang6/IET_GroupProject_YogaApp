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
            tutorialPage = new TutorialPage(this);
            categoryPage = cP;
        }
        public CategoryList()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            tutorialPage.Location = new Point((Screen.PrimaryScreen.WorkingArea.Width - tutorialPage.Width) / 2,
                          (Screen.PrimaryScreen.WorkingArea.Height - tutorialPage.Height) / 2);
            this.Hide();
            tutorialPage.Show();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            tutorialPage.Location = new Point((Screen.PrimaryScreen.WorkingArea.Width - tutorialPage.Width) / 2,
                          (Screen.PrimaryScreen.WorkingArea.Height - tutorialPage.Height) / 2);
            this.Hide();
            tutorialPage.Show();
        }
    }
}
