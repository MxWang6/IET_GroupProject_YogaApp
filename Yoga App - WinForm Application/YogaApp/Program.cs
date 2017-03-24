using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Drawing;

namespace YogaApp
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            CategoryPage categoryPage = new CategoryPage();
            categoryPage.Location = new Point((Screen.PrimaryScreen.WorkingArea.Width - categoryPage.Width) / 2,
                         (Screen.PrimaryScreen.WorkingArea.Height - categoryPage.Height) / 2);
            Application.Run(categoryPage);
        }
    }
}
