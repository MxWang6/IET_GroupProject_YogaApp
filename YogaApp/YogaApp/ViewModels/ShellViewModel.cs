﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MahApps.Metro.IconPacks;

namespace YogaApp.ViewModels
{
    internal class ShellViewModel : ViewModelBase
    {
        public ShellViewModel()
        {
            // Build the menus
            this.Menu.Add(new MenuItem() { Icon = new PackIconModern() { Kind = PackIconModernKind.HomeEmpty }, Text = "Home", NavigationDestination = new Uri("Views/MainPage.xaml", UriKind.RelativeOrAbsolute) });

            //this.Menu.Add(new MenuItem() { Icon = new PackIconFontAwesome() { Kind = PackIconFontAwesomeKind.Bug }, Text = "Bugs", NavigationDestination = new Uri("Views/BugsPage.xaml", UriKind.RelativeOrAbsolute) });
            //this.Menu.Add(new MenuItem() { Icon = new PackIconFontAwesome() { Kind = PackIconFontAwesomeKind.UserOutline }, Text = "User", NavigationDestination = new Uri("Views/UserPage.xaml", UriKind.RelativeOrAbsolute) });
            //this.Menu.Add(new MenuItem() { Icon = new PackIconFontAwesome() { Kind = PackIconFontAwesomeKind.Coffee }, Text = "Break", NavigationDestination = new Uri("Views/BreakPage.xaml", UriKind.RelativeOrAbsolute) });
            //this.Menu.Add(new MenuItem() { Icon = new PackIconFontAwesome() { Kind = PackIconFontAwesomeKind.FontAwesome }, Text = "Awesome", NavigationDestination = new Uri("Views/AwesomePage.xaml", UriKind.RelativeOrAbsolute) });

            //this.OptionsMenu.Add(new MenuItem() { Icon = new PackIconFontAwesome() { Kind = PackIconFontAwesomeKind.Cogs }, Text = "Settings", NavigationDestination = new Uri("Views/SettingsPage.xaml", UriKind.RelativeOrAbsolute) });
            //this.OptionsMenu.Add(new MenuItem() { Icon = new PackIconFontAwesome() { Kind = PackIconFontAwesomeKind.InfoCircle }, Text = "About", NavigationDestination = new Uri("Views/AboutPage.xaml", UriKind.RelativeOrAbsolute) });

            this.OptionsMenu.Add(new MenuItem() {Icon = new PackIconModern() {Kind = PackIconModernKind.Settings}, Text = "Settings", NavigationDestination = new Uri("Views/Settings.xaml", UriKind.RelativeOrAbsolute) });
            this.OptionsMenu.Add(new MenuItem() {Icon = new PackIconModern() {Kind = PackIconModernKind.Information}, Text = "About", NavigationDestination = new Uri("Views/About.xaml", UriKind.RelativeOrAbsolute) });
        }
    }
}
