using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace YogaApp.Views
{
    /// <summary>
    /// Interaction logic for Expert.xaml
    /// </summary>
    public partial class Expert : Page
    {
        public Expert()
        {
            InitializeComponent();
        }

        private void CowPose_OnClick(object sender, RoutedEventArgs e)
        {
            Navigation.Navigation.Navigate(new VideoIntro("CowPose"));
        }

        private void TrianglePose_OnClick(object sender, RoutedEventArgs e)
        {
            Navigation.Navigation.Navigate(new VideoIntro("TrianglePose"));
        }
    }
}
