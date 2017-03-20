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
using MahApps.Metro.Controls;
using MahApps.Metro.Controls.Dialogs;

namespace YogaApp.Pages
{
    /// <summary>
    /// Interaction logic for Home.xaml
    /// </summary>
    public partial class Home : Page
    {
        public Home()
        {
            InitializeComponent();
        }

        private void Start_Click(object sender, RoutedEventArgs e)
        {
            Catalogue catalogue= new Catalogue();
            NavigationService nav = NavigationService.GetNavigationService(this);
            nav.Navigate(catalogue);
        }

        private async void Exit_Click(object sender, RoutedEventArgs e)
        {
            var window = TreeHelper.TryFindParent<MainWindow>(this);
            var mySettings = new MetroDialogSettings()
            {
                AffirmativeButtonText = "Quit",
                NegativeButtonText = "Cancel",
                AnimateShow = true,
                AnimateHide = true
            };

            var result = await DialogManager.ShowMessageAsync(window, "Quit application?",
                "Sure you want to quit application?", MessageDialogStyle.AffirmativeAndNegative, mySettings);
            var shutdown = result == MessageDialogResult.Affirmative;
            if (shutdown)
            {
                Application.Current.Shutdown();
            }
        }
    }
}
