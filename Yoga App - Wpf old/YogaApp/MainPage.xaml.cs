﻿using System;
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

namespace WpfApplication1
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

        private void navigateToTutorial()
        {
            NavigationService navService = NavigationService.GetNavigationService(this);
            navService.Navigate(new System.Uri("TutorialPage.xaml", UriKind.RelativeOrAbsolute));
        }

        private void btnChild_Click(object sender, RoutedEventArgs e)
        {
            navigateToTutorial();
        }

        private void btnSenior_Click(object sender, RoutedEventArgs e)
        {
            navigateToTutorial();
        }

        private void btnmale_Click(object sender, RoutedEventArgs e)
        {
            navigateToTutorial();
        }

        private void btnFemale_Click(object sender, RoutedEventArgs e)
        {
            navigateToTutorial();
        }
    }
}