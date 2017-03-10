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
    public partial class CategoryList : Form
    {
        CategoryPage categoryPage;
        public CategoryList(CategoryPage cP)
        {
            InitializeComponent();
            categoryPage = cP;
        }
        public CategoryList()
        {
            InitializeComponent();
        }
    }
}
