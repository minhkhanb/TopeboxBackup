using Microsoft.Phone.Controls;
using Pine.Dialogs;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Media;

namespace Pine {
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
        AccountAskDialogType _type;
        override public Popup Dialog { get { return _popup; } }
        public AccountAskDialog(AccountAskDialogType type) {
            _type = type;
            _popup = new Popup();
            createContent((App.Current.RootVisual as PhoneApplicationFrame).Orientation);
            //
        }
        void createContent(PageOrientation ori)
        {
            float page_width = (float)Application.Current.Host.Content.ActualWidth;
            float page_height = (float)Application.Current.Host.Content.ActualHeight;
            if (ori == PageOrientation.Portrait || ori == PageOrientation.PortraitUp || ori == PageOrientation.PortraitDown)
            {
                float tmp = page_width;
                page_width = page_width > page_height ? page_height : page_width;
                page_height = page_width == page_height ? tmp : page_height;
            }
            else
            {
                float tmp = page_width;
                page_width = page_width > page_height ? page_width : page_height;
                page_height = page_width == page_height ? tmp : page_height;
            }


            Border border_root = new Border();
            border_root.Margin = new Thickness(0, 0, 0, 0);
            //border_root.CornerRadius = new CornerRadius(8);
           
            border_root.Width = page_width;
            //border_root.Height = ;
            border_root.Background = new SolidColorBrush(Color.FromArgb(0xFF, 0x00, 0xA1, 0x4E));

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
            panel_content.Children.Add(content);
            if (_type == AccountAskDialogType.TypeOkLateNever)
            {
                Grid grid_btn = new Grid();
                
                if (ori == PageOrientation.Landscape || ori == PageOrientation.LandscapeLeft || ori == PageOrientation.LandscapeRight)
                {
                    grid_btn.Margin = new Thickness(0, 50, 0, 0);
                    grid_btn.Width = page_width - 50;
                    grid_btn.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(200, GridUnitType.Pixel) });
                    grid_btn.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(250, GridUnitType.Pixel) });
                    grid_btn.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) });
                    panel_content.Children.Add(grid_btn);
                }

                Button btnOk = new Button();
                btnOk.Content = "Ok";
                btnOk.Name = "Ok";
                //btnOk.Height = 55;
                //btnOk.Width = 100;
                btnOk.Padding = new Thickness(5, 5, 5, 5);
                btnOk.Click += btnOk_Click;

                //btnOk.FontSize = 34;

                if (ori == PageOrientation.Landscape || ori == PageOrientation.LandscapeLeft || ori == PageOrientation.LandscapeRight)
                {
                    grid_btn.Children.Add(btnOk);
                    Grid.SetColumn(btnOk, 0);
                }
                else
                {
                    panel_content.Children.Add(btnOk);
                }

                Button btnLate = new Button();
                btnLate.Content = "Later!";
                //btnLate.Width = 150;
                //btnLate.Height = 35;
                btnLate.Padding = new Thickness(5, 5, 5, 5);
                btnLate.Click += btnLate_Click;

                if (ori == PageOrientation.Landscape || ori == PageOrientation.LandscapeLeft || ori == PageOrientation.LandscapeRight)
                {
                    grid_btn.Children.Add(btnLate);
                    Grid.SetColumn(btnLate, 1);
                }
                else
                {
                    panel_content.Children.Add(btnLate);
                }

                Button btnNotShow = new Button();
                btnNotShow.Content = "Never!";
                //btnNotShow.Height = 35;
                //btnNotShow.Width = 300;
                btnNotShow.Padding = new Thickness(5, 5, 5, 5);
                btnNotShow.Click += btnNotShow_Click;

                if (ori == PageOrientation.Landscape || ori == PageOrientation.LandscapeLeft || ori == PageOrientation.LandscapeRight)
                {
                    grid_btn.Children.Add(btnNotShow);
                    Grid.SetColumn(btnNotShow, 2);
                }
                else
                {
                    panel_content.Children.Add(btnNotShow);
                }
            }
            else if (_type == AccountAskDialogType.TypeOkLate)
            {
                Grid grid_btn = new Grid();
                grid_btn.Margin = new Thickness(0, 50, 0, 0);
                grid_btn.Width = Application.Current.Host.Content.ActualWidth - 50;
                grid_btn.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(375, GridUnitType.Pixel) });
                grid_btn.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) });
                panel_content.Children.Add(grid_btn);
                Button btnOk = new Button();
                btnOk.Content = "Ok";
                btnOk.Name = "Ok";
                //btnOk.Height = 55;
                //btnOk.Width = 100;
                btnOk.Padding = new Thickness(5, 5, 5, 5);
                btnOk.Click += btnOk_Click;

                //btnOk.FontSize = 34;
                grid_btn.Children.Add(btnOk);
                Grid.SetColumn(btnOk, 0);

                Button btnLate = new Button();
                btnLate.Content = "Later!";
                //btnLate.Width = 150;
                //btnLate.Height = 35;
                btnLate.Padding = new Thickness(5, 5, 5, 5);
                btnLate.Click += btnLate_Click;
                grid_btn.Children.Add(btnLate);
                Grid.SetColumn(btnLate, 1);
            }
            else if (_type == AccountAskDialogType.TypeOk)
            {
                Button btnOk = new Button();
                btnOk.Content = "Ok";
                btnOk.Name = "Ok";
                //btnOk.Height = 55;
                //btnOk.Width = 100;
                btnOk.Margin = new Thickness(0, 50, 0, 0);
                btnOk.Padding = new Thickness(5, 5, 5, 5);
                btnOk.Width = 300;
                btnOk.Click += btnOk_Click;
                panel_content.Children.Add(btnOk);
            }
            border_content.Child = panel_content;
            grid_root.Children.Add(border_content);
            border_root.Child = grid_root;
            _popup.Child = border_root;
        }
        void btnNotShow_Click(object sender, RoutedEventArgs e) {
            if (OnComplete != null) OnComplete(this, new AccountAskDialogArgs(AccountAskDialogResult.NOT_SHOW));
            _popup.IsOpen = false;
            AppObject.OnOrientationChange -= AppObject_OnOrientationChange;
        }

        void btnLate_Click(object sender, RoutedEventArgs e) {
            if (OnComplete != null) OnComplete(this, new AccountAskDialogArgs(AccountAskDialogResult.LATE));
            _popup.IsOpen = false;
            AppObject.OnOrientationChange -= AppObject_OnOrientationChange;
        }

        void btnOk_Click(object sender, RoutedEventArgs e) {
            if (OnComplete != null) OnComplete(this, new AccountAskDialogArgs(AccountAskDialogResult.OK));
            _popup.IsOpen = false;
            AppObject.OnOrientationChange -= AppObject_OnOrientationChange;
        }
        void AppObject_OnOrientationChange(object sender, PageOrientation page_ori)
        {
            createContent(page_ori);
        }
        public void show() {
            _popup.IsOpen = true;
            AppObject.OnOrientationChange += AppObject_OnOrientationChange;
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
            createContent((App.Current.RootVisual as PhoneApplicationFrame).Orientation);
            //
        }
        void createContent(PageOrientation ori)
        {
            float page_width = (float)Application.Current.Host.Content.ActualWidth;
            float page_height = (float)Application.Current.Host.Content.ActualHeight;
            if (ori == PageOrientation.Portrait || ori == PageOrientation.PortraitUp || ori == PageOrientation.PortraitDown)
            {
                float tmp = page_width;
                page_width = page_width > page_height ? page_height : page_width;
                page_height = page_width == page_height ? tmp : page_height;
            }
            else
            {
                float tmp = page_width;
                page_width = page_width > page_height ? page_width : page_height;
                page_height = page_width == page_height ? tmp : page_height;
            }
            Border border_root = new Border();
            border_root.Margin = new Thickness(0, 0, 0, 0);
            border_root.Width = page_width;
            //border_root.Height = 300;
            border_root.Background = new SolidColorBrush(Color.FromArgb(0xFF, 0x00, 0xA1, 0x4E));

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
            content.Foreground = new SolidColorBrush(Color.FromArgb(0xFF, 0xFF, 0, 0));
            panel_content.Children.Add(content);

            Button btnOk = new Button();
            btnOk.Content = "Ok";
            btnOk.Name = "Ok";
            //btnOk.Height = 55;
            //btnOk.Width = 100;
            btnOk.Margin = new Thickness(0, 50, 0, 0);
            btnOk.Padding = new Thickness(5, 5, 5, 5);
            btnOk.Width = 300;
            btnOk.Click += btnOk_Click;
            panel_content.Children.Add(btnOk);

            border_content.Child = panel_content;
            grid_root.Children.Add(border_content);
            border_root.Child = grid_root;
            _popup.Child = border_root;
        }
        void AppObject_OnOrientationChange(object sender, PageOrientation page_ori)
        {
            createContent(page_ori);
        }
        void btnOk_Click(object sender, RoutedEventArgs e) {
            if (OnComplete != null) OnComplete(this, new AccountRequireDialogArgs());
            _popup.IsOpen = false;
            AppObject.OnOrientationChange -= AppObject_OnOrientationChange;
        }
        public void show() {
            _popup.IsOpen = true;
            AppObject.OnOrientationChange += AppObject_OnOrientationChange;
            _popup.VerticalOffset = 0;
        }
    }
}
