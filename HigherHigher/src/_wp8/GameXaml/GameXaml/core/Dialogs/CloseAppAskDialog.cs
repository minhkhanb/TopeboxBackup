using Microsoft.Phone.Controls;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Media;

namespace Pine.Dialogs {
    enum CloseAppAskDialogResult {
        UNKNOWN = 0,
        OK = 1,
        CANCEL = 2,
    }

    class CloseAppAskDialogArgs {
        public CloseAppAskDialogResult Result = CloseAppAskDialogResult.CANCEL;
        public CloseAppAskDialogArgs(CloseAppAskDialogResult result) {
            Result = result;
        }
    }

    delegate void DelegateCloseAppAskDialogComplete(object sender, CloseAppAskDialogArgs a);
    class CloseAppAskDialog : DialogBase
    {
        public event DelegateCloseAppAskDialogComplete OnComplete;
        
        Popup _popup;
        public bool isShow { get { return _popup.IsOpen; } }
        override public Popup Dialog { get { return _popup; } }
        public CloseAppAskDialog() {
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
            //border_root.Height = 250;
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
            title.Text = "Exit game";
            title.FontSize = 30;
            panel_content.Children.Add(title);

            TextBlock content = new TextBlock();
            content.Margin = new Thickness(0, 25, 0, 0);
            content.TextWrapping = TextWrapping.Wrap;
            content.Text = "Do you want to exit game?";
            content.FontSize = 25;
            panel_content.Children.Add(content);

            Grid grid_btn = new Grid();
            if (ori == PageOrientation.Landscape || ori == PageOrientation.LandscapeLeft || ori == PageOrientation.LandscapeRight)
            {
                grid_btn.Margin = new Thickness(0, 25, 0, 0);
                grid_btn.Width = 600;
                grid_btn.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(200, GridUnitType.Pixel) });
                grid_btn.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(200, GridUnitType.Pixel) });
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

            Button btnCancel = new Button();
            btnCancel.Content = "Cancel";
            //btnLate.Width = 150;
            //btnLate.Height = 35;
            btnCancel.Padding = new Thickness(5, 5, 5, 5);
            btnCancel.Click += btnCancel_Click;
            if (ori == PageOrientation.Landscape || ori == PageOrientation.LandscapeLeft || ori == PageOrientation.LandscapeRight)
            {
                grid_btn.Children.Add(btnCancel);
                Grid.SetColumn(btnCancel, 2);
            }
            else
            {
                panel_content.Children.Add(btnCancel);
            }

            border_content.Child = panel_content;
            if (ori == PageOrientation.Landscape || ori == PageOrientation.LandscapeLeft || ori == PageOrientation.LandscapeRight)
            {
                grid_root.Children.Add(border_content);
                border_root.Child = grid_root;
            }
            else
            {
                border_root.Child = border_content;
            }
            _popup.Child = border_root;
        }
        void AppObject_OnOrientationChange(object sender, PageOrientation page_ori)
        {
            createContent(page_ori);
        }
        void btnCancel_Click(object sender, RoutedEventArgs e) {
            _popup.IsOpen = false;
            AppObject.OnOrientationChange -= AppObject_OnOrientationChange;
            if (OnComplete != null) OnComplete(this, new CloseAppAskDialogArgs(CloseAppAskDialogResult.CANCEL));
            
        }

        void btnOk_Click(object sender, RoutedEventArgs e) {
            _popup.IsOpen = false;
            AppObject.OnOrientationChange -= AppObject_OnOrientationChange;
            if (OnComplete != null) OnComplete(this, new CloseAppAskDialogArgs(CloseAppAskDialogResult.OK));
            
        }
        public void show() {
            _popup.IsOpen = true;
            AppObject.OnOrientationChange += AppObject_OnOrientationChange;
            _popup.VerticalOffset = 0;
        }
        public void hide() {
            _popup.IsOpen = false;
        }
    }
}
