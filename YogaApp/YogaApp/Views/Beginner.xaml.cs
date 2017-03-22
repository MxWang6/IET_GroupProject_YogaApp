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
        }

        private void Tile1_OnClick(object sender, RoutedEventArgs e)
        {
            Navigation.Navigation.Navigate(new Uri("Views/Video.xaml", UriKind.RelativeOrAbsolute));
        }

        private void Tile2_OnClick(object sender, RoutedEventArgs e)
        {
            Navigation.Navigation.Navigate(new Uri("Views/Video.xaml", UriKind.RelativeOrAbsolute));
        }
    }
}
