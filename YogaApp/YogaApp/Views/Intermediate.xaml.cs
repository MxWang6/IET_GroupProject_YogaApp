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
    /// Interaction logic for Intermediate.xaml
    /// </summary>
    public partial class Intermediate : Page
    {
        public Intermediate()
        {
            InitializeComponent();
        }

        private void BowPose_OnClick(object sender, RoutedEventArgs e)
        {
            Navigation.Navigation.Navigate(new VideoIntro("BowPose"));
        }

        private void BridgePose_OnClick(object sender, RoutedEventArgs e)
        {
            Navigation.Navigation.Navigate(new VideoIntro("BridgePose"));
        }
    }
}
