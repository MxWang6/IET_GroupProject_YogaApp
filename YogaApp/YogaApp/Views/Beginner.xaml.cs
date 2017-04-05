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
    /// Interaction logic for Beginner.xaml
    /// </summary>
    public partial class Beginner : Page
    {
        public Beginner()
        {
            InitializeComponent();
            //var name = value;
        }

        private void TreePose_OnClick(object sender, RoutedEventArgs e)
        {
            //Navigation.Navigation.Navigate(new Uri("Views/Video.xaml", UriKind.RelativeOrAbsolute));
            Navigation.Navigation.Navigate(new VideoIntro("TreePose"));
        }

        private void CatPose_OnClick(object sender, RoutedEventArgs e)
        {
            //Navigation.Navigation.Navigate(new Uri("Views/Video.xaml", UriKind.RelativeOrAbsolute));
            Navigation.Navigation.Navigate(new VideoIntro("CatPose"));
        }
    }
}
