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
            AddPosesToForm();
        }

        private void AddPosesToForm()
        {
            string[] categories = ExcelReader.getExcelFile("PoseData.xlsx", "Beginner").Split(',');
            for (int i = 0; i < categories.Length; i++)
            {
                string[] categoryDetails = categories[i].Split(':');
                Button button = new Button();
                button.Location = new Point(273, 19 + (58*i));
                button.Size = new Size(160, 34);
                button.TabIndex = 0;
                button.UseVisualStyleBackColor = true;
                button.Click += new EventHandler(button_Click);
                button.Text = categoryDetails[0];
                button.Name = categoryDetails[1];
                groupBox1.Controls.Add(button);
            }
        }

        private void navigateToTutorialPage()
        {
            this.Hide();
            tutorialPage.Location = new Point((Screen.PrimaryScreen.WorkingArea.Width - tutorialPage.Width) / 2,
                          (Screen.PrimaryScreen.WorkingArea.Height - tutorialPage.Height) / 2);
            tutorialPage.Show();
            
        }

        private void button_Click(object sender, EventArgs e)
        {
            navigateToTutorialPage();
            var button = (Button)sender;
            string Name = button.Name;
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
    }
}
