using System;
using System.Drawing;
using System.Windows.Forms;
using YogaApp.Utils;


namespace YogaApp
{
    public partial class CategoryList : Form
    {
        TutorialPage tutorialPage;
        CategoryPage categoryPage;
        public string videoDescription = null;

        private static Button exchangeButton;
        

        public Button getName()
        {
            return exchangeButton;
        } 

        public void setName(string name)
        {
            exchangeButton.Name = name;
        }

        public CategoryList(CategoryPage cP)
        {
            InitializeComponent();
            categoryPage = cP;
            tutorialPage = new TutorialPage(this);
            exchangeButton = new Button();
        }

        private void navigateToTutorialPage()
        {
            this.Hide();
            tutorialPage.Location = new Point((Screen.PrimaryScreen.WorkingArea.Width - tutorialPage.Width) / 2,
                          (Screen.PrimaryScreen.WorkingArea.Height - tutorialPage.Height) / 2);
            tutorialPage.Show();
            
        }

        private void buttonTree_Click(object sender, EventArgs e)
        {
            navigateToTutorialPage();
            var button = (Button)sender;
            String Name = button.Name;
            setName(Name);
            tutorialPage.richTextBox1.Text = videoDescription;
            Console.WriteLine(getName()); 

        }

        private void button2_Click(object sender, EventArgs e)
        {
            navigateToTutorialPage();
            var button = (Button)sender;
            String Name = button.Name;
            setName(Name);
            tutorialPage.richTextBox1.Text = videoDescription;
            Console.WriteLine(getName());

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
