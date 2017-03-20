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

namespace YogaApp.Pages
{
    /// <summary>
    /// Interaction logic for Catalogue.xaml
    /// </summary>
    public partial class Catalogue : Page
    {
        public Catalogue()
        {
            InitializeComponent();
        }

        private void navigateToTutorial()
        {
            Tutorial tutorial = new Tutorial();
            NavigationService nav = NavigationService.GetNavigationService(this);
            nav.Navigate(tutorial);
        }

        private void btnChild_Click(object sender, RoutedEventArgs e)
        {
            navigateToTutorial();
        }

        private void btnSenior_Click(object sender, RoutedEventArgs e)
        {
            navigateToTutorial();
        }

        private void btnMale_Click(object sender, RoutedEventArgs e)
        {
            navigateToTutorial();
        }

        private void btnFemale_Click(object sender, RoutedEventArgs e)
        {
            navigateToTutorial();
        }
    }
}
