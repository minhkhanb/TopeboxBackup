using Pine.Dialogs;
using PineShap.Dialogs;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using Windows.UI;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Media;

namespace PineShap {
    enum AccountAskDialogResult {
        UNKNOWN = 0,
        OK = 1,
        LATE = 2,
        NOT_SHOW = 3,
    }
    class AccountAskDialogArgs{
        public AccountAskDialogResult Result = AccountAskDialogResult.NOT_SHOW;
        public AccountAskDialogArgs(AccountAskDialogResult result) {
            Result = result;
        }
    }
    delegate void DelegateAccountAskDialogComplete(object sender, AccountAskDialogArgs a);
    public enum AccountAskDialogType
    {
        TypeOk,
        TypeOkLate,
        TypeOkLateNever
    }
    class AccountAskDialog : DialogBase
    {
        public event DelegateAccountAskDialogComplete OnComplete;
        Popup _popup;
        override public Popup Dialog { get { return _popup; } }
        public AccountAskDialog(AccountAskDialogType type) {
            _popup = new Popup();
            Border border_root = new Border();
           
            //border_root.CornerRadius = new CornerRadius(8);
            float page_width = (float)Window.Current.Bounds.Width;
            float page_height = (float)Window.Current.Bounds.Height;
            float content_width = page_width > 800 ? 800 : page_width;
            border_root.Width = content_width;//// Application.Current.Host.Content.ActualHeight;
            //border_root.Height = 300;
            border_root.Margin = new Thickness(page_width > 800?(page_width - 800.0f)/2.0f:0, 0, 0, 0);
            border_root.Background = new SolidColorBrush(Color.FromArgb(0xFF,0x00,0xA1,0x4E));

            Grid grid_root = new Grid();
            grid_root.RowDefinitions.Add(new RowDefinition() { Height = new GridLength(1, GridUnitType.Auto) });
            grid_root.RowDefinitions.Add(new RowDefinition() { Height = new GridLength(1, GridUnitType.Star) });

            Border border_content = new Border();
            border_content.VerticalAlignment = VerticalAlignment.Stretch;
            border_content.HorizontalAlignment = HorizontalAlignment.Stretch;
            border_content.Margin = new Thickness(15, 15, 15, 15);

            StackPanel panel_content = new StackPanel();
            panel_content.HorizontalAlignment = HorizontalAlignment.Center;
            panel_content.VerticalAlignment = VerticalAlignment.Stretch;
            panel_content.HorizontalAlignment = HorizontalAlignment.Stretch;
            TextBlock title = new TextBlock();
            title.TextAlignment = TextAlignment.Center;
            title.Height = 55;
            title.Text = AppObject.GameInfo.AccountDialogTitle;
            title.FontSize = 30;
            panel_content.Children.Add(title);

            TextBlock content = new TextBlock();
            content.TextWrapping = TextWrapping.Wrap;
            content.Text = AppObject.GameInfo.AccountDialogMessage;
            content.TextAlignment = TextAlignment.Center;
            panel_content.Children.Add(content);
            if (type == AccountAskDialogType.TypeOkLateNever)
            {
                Grid grid_btn = new Grid();
                grid_btn.Margin = new Thickness(0, 50, 0, 0);
                grid_btn.Width = content_width;
                grid_btn.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(content_width/3, GridUnitType.Pixel) });
                grid_btn.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(content_width/3, GridUnitType.Pixel) });
                grid_btn.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) });
                panel_content.Children.Add(grid_btn);
                Button btnOk = new Button();
                btnOk.Content = "Ok";
                btnOk.Name = "Ok";
                //btnOk.Height = 55;
                //btnOk.Width = 100;
                btnOk.Padding = new Thickness(15, 5, 15, 5);
                btnOk.HorizontalAlignment = HorizontalAlignment.Center;
                btnOk.Click += btnOk_Click;

                //btnOk.FontSize = 34;
                grid_btn.Children.Add(btnOk);
                Grid.SetColumn(btnOk, 0);

                Button btnLate = new Button();
                btnLate.Content = "Later!";
                //btnLate.Width = 150;
                //btnLate.Height = 35;
                btnLate.Padding = new Thickness(15, 5, 15, 5);
                btnLate.Click += btnLate_Click;
                btnLate.HorizontalAlignment = HorizontalAlignment.Center;
                grid_btn.Children.Add(btnLate);
                Grid.SetColumn(btnLate, 1);

                Button btnNotShow = new Button();
                btnNotShow.Content = "Never!";
                //btnNotShow.Height = 35;
                //btnNotShow.Width = 300;
                btnNotShow.Padding = new Thickness(15, 5, 15, 5);
                btnNotShow.Click += btnNotShow_Click;
                btnNotShow.HorizontalAlignment = HorizontalAlignment.Center;
                grid_btn.Children.Add(btnNotShow);
                Grid.SetColumn(btnNotShow, 2);
            }
            else if (type == AccountAskDialogType.TypeOkLate)
            {
                Grid grid_btn = new Grid();
                grid_btn.Margin = new Thickness(0, 25, 0, 0);
                grid_btn.Width = content_width;
                grid_btn.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(content_width/2, GridUnitType.Pixel) });
                grid_btn.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) });

                panel_content.Children.Add(grid_btn);
                Button btnOk = new Button();
                btnOk.Content = "Ok";
                btnOk.Name = "Ok";
                btnOk.HorizontalAlignment = HorizontalAlignment.Center;
                //btnOk.Height = 55;
                //btnOk.Width = 100;
                btnOk.Padding = new Thickness(15, 5, 15, 5);
                btnOk.Click += btnOk_Click;

                //btnOk.FontSize = 34;
                grid_btn.Children.Add(btnOk);
                Grid.SetColumn(btnOk, 0);

                Button btnLate = new Button();
                btnLate.Content = "Later!";
                //btnLate.Width = 150;
                //btnLate.Height = 35;
                btnLate.Padding = new Thickness(15, 5, 15, 5);
                btnLate.Click += btnLate_Click;
                btnLate.HorizontalAlignment = HorizontalAlignment.Center;
                grid_btn.Children.Add(btnLate);
                Grid.SetColumn(btnLate, 1);
            }
            else if (type == AccountAskDialogType.TypeOk)
            {
                Button btnOk = new Button();
                btnOk.Content = "Ok";
                btnOk.Name = "Ok";
                //btnOk.Height = 55;
                //btnOk.Width = 100;
                btnOk.Margin = new Thickness(0, 25, 0, 0);
                btnOk.Padding = new Thickness(15, 5, 15, 5);
                btnOk.Width = 300;
                btnOk.Click += btnOk_Click;
                panel_content.Children.Add(btnOk);
            }
            border_content.Child = panel_content;
            grid_root.Children.Add(border_content);
            border_root.Child = grid_root;
            _popup.Child = border_root;
            //
        }

        void btnNotShow_Click(object sender, RoutedEventArgs e) {
            if (OnComplete != null) OnComplete(this, new AccountAskDialogArgs(AccountAskDialogResult.NOT_SHOW));
            _popup.IsOpen = false;
        }

        void btnLate_Click(object sender, RoutedEventArgs e) {
            if (OnComplete != null) OnComplete(this, new AccountAskDialogArgs(AccountAskDialogResult.LATE));
            _popup.IsOpen = false;
        }

        void btnOk_Click(object sender, RoutedEventArgs e) {
            if (OnComplete != null) OnComplete(this, new AccountAskDialogArgs(AccountAskDialogResult.OK));
            _popup.IsOpen = false;
        }
        public void show() {
            _popup.IsOpen = true;
            _popup.VerticalOffset = 0;
        }
    }
    class AccountRequireDialogArgs
    {
        public AccountRequireDialogArgs()
        {
            
        }
    }
    delegate void DelegateAccountRequireDialogComplete(object sender, AccountRequireDialogArgs a);
    class AccountRequireDialog : DialogBase
    {
        public event DelegateAccountRequireDialogComplete OnComplete;
        Popup _popup;
        override public Popup Dialog { get { return _popup; } }
        public AccountRequireDialog() {
            _popup = new Popup();
            Border border_root = new Border();
            border_root.Margin = new Thickness(0, 0, 0, 0);
            float page_width = (float)Window.Current.Bounds.Width;
            float page_height = (float)Window.Current.Bounds.Height;
            float content_width = page_width > 800 ? 800 : page_width;
            border_root.Width = content_width;//// Application.Current.Host.Content.ActualHeight;
            //border_root.Height = 300;
            border_root.Background = new SolidColorBrush(Color.FromArgb(0xFF,0x00,0xA1,0x4E));

            Grid grid_root = new Grid();
            grid_root.RowDefinitions.Add(new RowDefinition() { Height = new GridLength(1, GridUnitType.Auto) });
            grid_root.RowDefinitions.Add(new RowDefinition() { Height = new GridLength(1, GridUnitType.Star) });

            Border border_content = new Border();
            border_content.VerticalAlignment = VerticalAlignment.Stretch;
            border_content.HorizontalAlignment = HorizontalAlignment.Stretch;
            border_content.Margin = new Thickness(15, 15, 15, 15);

            StackPanel panel_content = new StackPanel();
            panel_content.HorizontalAlignment = HorizontalAlignment.Center;
            panel_content.VerticalAlignment = VerticalAlignment.Stretch;
            panel_content.HorizontalAlignment = HorizontalAlignment.Stretch;
            TextBlock title = new TextBlock();
            title.TextAlignment = TextAlignment.Center;
            title.Height = 55;
            title.Text = "Warning!";
            title.FontSize = 30;
            title.Foreground = new SolidColorBrush(Color.FromArgb(0xFF, 0xFF, 0, 0));
            panel_content.Children.Add(title);

            TextBlock content = new TextBlock();
            content.TextWrapping = TextWrapping.Wrap;
            content.Text = "Please make sure that you play Pocket Avenger with strong network connection! Pocket Avenger requires your Facebook account as login.";
            content.TextAlignment = TextAlignment.Center;
            content.Foreground = new SolidColorBrush(Color.FromArgb(0xFF, 0xFF, 0, 0));
            panel_content.Children.Add(content);
            
            Button btnOk = new Button();
            btnOk.Content = "Ok";
            btnOk.Name = "Ok";
            //btnOk.Height = 55;
            //btnOk.Width = 100;
            btnOk.Margin = new Thickness(0, 50, 0, 0);
            btnOk.Padding = new Thickness(5, 5, 5, 5);
            btnOk.HorizontalAlignment = HorizontalAlignment.Center;
            btnOk.Width = 300;
            btnOk.Click += btnOk_Click;
            panel_content.Children.Add(btnOk);

            border_content.Child = panel_content;
            grid_root.Children.Add(border_content);
            border_root.Child = grid_root;
            _popup.Child = border_root;
            //
        }

        void btnOk_Click(object sender, RoutedEventArgs e) {
            if (OnComplete != null) OnComplete(this, new AccountRequireDialogArgs());
            _popup.IsOpen = false;
        }
        public void show() {
            _popup.IsOpen = true;
            _popup.VerticalOffset = 0;
        }
    }
}
