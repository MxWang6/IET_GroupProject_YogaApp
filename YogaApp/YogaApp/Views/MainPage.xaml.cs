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
    /// Interaction logic for MainPage.xaml
    /// </summary>
    public partial class MainPage : Page
    {
        public MainPage()
        {
            InitializeComponent();
        }

        private void Beginner_Click(object sender, RoutedEventArgs e)
        {
            Navigation.Navigation.Navigate(new Uri("Views/Beginner.xaml", UriKind.RelativeOrAbsolute));
        }

        private void Intermediate_Click(object sender, RoutedEventArgs e)
        {
            Navigation.Navigation.Navigate(new Uri("Views/Intermediate.xaml", UriKind.RelativeOrAbsolute));
        }

        private void Advanced_Click(object sender, RoutedEventArgs e)
        {
            Navigation.Navigation.Navigate(new Uri("Views/Advanced.xaml", UriKind.RelativeOrAbsolute));
        }

        private void Expert_Click(object sender, RoutedEventArgs e)
        {
            Navigation.Navigation.Navigate(new Uri("Views/Expert.xaml", UriKind.RelativeOrAbsolute));
        }
    }
}
