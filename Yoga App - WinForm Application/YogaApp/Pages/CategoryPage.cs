using System;
using System.Drawing;
using System.Windows.Forms;
using YogaApp.Utils;

namespace YogaApp
{
    public partial class CategoryPage : Form
    {
        CategoryList categoryList;
        public CategoryPage()
        {
            InitializeComponent();
            categoryList = new CategoryList(this);
            Console.WriteLine(ExcelReader.getExcelFile("PoseData.xlsx", "surname"));
            CsvWriter.addPoseNameToCsv("Tree Pose");
        }

        private void navigateToCategoryListing()
        {
            this.Hide();
            categoryList.Location = new Point((Screen.PrimaryScreen.WorkingArea.Width - categoryList.Width) / 2,
                          (Screen.PrimaryScreen.WorkingArea.Height - categoryList.Height) / 2);
            categoryList.Show();
        }

        private void childButton_Click(object sender, EventArgs e)
        {
            navigateToCategoryListing();

        }

        private void button2_Click(object sender, EventArgs e)
        {
            navigateToCategoryListing();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            navigateToCategoryListing();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            navigateToCategoryListing();
        }


    }
}
