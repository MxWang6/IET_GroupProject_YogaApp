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
    public partial class TutorialPage : Form
    {
        public TutorialPage()
        {
            InitializeComponent();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            string path = "https://www.youtube.com/v/rSgIZ4wW8Fw?autoplay=1";
            axShockwaveFlash1.LoadMovie(0, path);
        }
    }
}
