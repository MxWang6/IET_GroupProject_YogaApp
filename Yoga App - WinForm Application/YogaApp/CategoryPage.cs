using System;
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
    public partial class CategoryPage : Form
    {
        TutorialPage tutorialPage;
        public CategoryPage()
        {
            InitializeComponent();
            tutorialPage = new TutorialPage(this);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.Hide();
            tutorialPage.Show();

        }
    }
}
