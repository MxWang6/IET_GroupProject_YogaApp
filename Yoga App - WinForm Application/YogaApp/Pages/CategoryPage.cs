using System;
using System.Drawing;
using System.Windows.Forms;
using YogaApp.Utils;

namespace YogaApp
{
    public partial class CategoryPage : Form
    {
        CategoryList categoryList;

        private static Button categoryButton;

        public Button getCategoryName()
        {
            return categoryButton;
        }

        public void setCategoryName(string name)
        {
            categoryButton.Name = name;
        }
        public CategoryPage()
        {
            InitializeComponent();
            //categoryList = new CategoryList(this);
            categoryButton = new Button();
            CsvWriter.addPoseNameToCsv("Tree Pose");
        }

        private void navigateToCategoryListing(object sender)
        {
            var button = (Button)sender;
            string Name = button.Text;
            setCategoryName(Name);
            categoryList = new CategoryList(this);
            this.Hide();
            categoryList.Location = new Point((Screen.PrimaryScreen.WorkingArea.Width - categoryList.Width) / 2,
                          (Screen.PrimaryScreen.WorkingArea.Height - categoryList.Height) / 2);
            categoryList.Show();
        }

        private void childButton_Click(object sender, EventArgs e)
        {
            navigateToCategoryListing(sender);

        }

        private void button2_Click(object sender, EventArgs e)
        {
            navigateToCategoryListing(sender);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            navigateToCategoryListing(sender);
        }

        private void button4_Click(object sender, EventArgs e)
        {
            navigateToCategoryListing(sender);
        }


    }
}
