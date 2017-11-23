using Microsoft.Phone.Controls;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Input;
using System.Windows.Media;

namespace Pine.Dialogs
{
    #region SUPPORT ASK
    public class SupportAccountAskEventArgs
    {
        public string AccountId;
    }
    public delegate void DelegateSupportAccountAskComplete(object sender, SupportAccountAskEventArgs e);
    public class SupportAccountAskDialog
    {


        public event DelegateSupportAccountAskComplete OnComplete = null;
        Popup _popup;
        public Popup Dialog { get { return _popup; } }
        public TextBox _id = new TextBox();
        public SupportAccountAskDialog()
        {
            _popup = new Popup();
            createContent((App.Current.RootVisual as PhoneApplicationFrame).Orientation);
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

            //BEGIN CONTENT
            TextBlock title = new TextBlock();
            title.TextAlignment = TextAlignment.Center;
            title.Height = 55;
            title.Text = "Support";
            title.FontSize = 30;
            panel_content.Children.Add(title);

            //current id
            TextBlock current = new TextBlock();
            current.TextWrapping = TextWrapping.Wrap;
            current.Text = "";
            panel_content.Children.Add(current);

            //add money
            TextBlock _id_title = new TextBlock();
            _id_title.Text = "Account ID:";
            panel_content.Children.Add(_id_title);

            //TextBox _id = new TextBox();
            panel_content.Children.Add(_id);
            InputScope scope = new InputScope();
            InputScopeName name = new InputScopeName();
            name.NameValue = InputScopeNameValue.Text;
            scope.Names.Add(name);
            _id.InputScope = scope;

            //button
            Grid grid_btn = new Grid();
            grid_btn.Margin = new Thickness(0, 50, 0, 0);
            grid_btn.Width = page_width;
            //grid_btn.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(200, GridUnitType.Pixel) });
            //grid_btn.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(250, GridUnitType.Pixel) });
            //grid_btn.ColumnDefinitions.Add(new ColumnDefinition() { Width = new GridLength(1, GridUnitType.Star) });
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

            border_content.Child = panel_content;
            grid_root.Children.Add(border_content);
            border_root.Child = grid_root;
            _popup.Child = border_root;
        }

        void btnOk_Click(object sender, RoutedEventArgs e)
        {
            if (OnComplete != null) OnComplete(this, new SupportAccountAskEventArgs() { AccountId = _id.Text });

            _popup.IsOpen = false;
            AppObject.OnOrientationChange -= AppObject_OnOrientationChange;
        }
        
        public void show()
        {
            _popup.IsOpen = true;
            AppObject.OnOrientationChange += AppObject_OnOrientationChange;
            _popup.VerticalOffset = 0;
        }
        void AppObject_OnOrientationChange(object sender, PageOrientation page_ori)
        {
            createContent(page_ori);
        }
    }
    #endregion
}
