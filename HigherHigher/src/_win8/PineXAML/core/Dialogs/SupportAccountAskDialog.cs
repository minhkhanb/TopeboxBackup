using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using Windows.UI;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;

namespace Pine.Dialogs
{
    #region SUPPORT ASK
    public class SupportAccountAskEvents
    {
        public UInt64 AccountId;
    }
    public delegate void DelegateSupportAccountAskComplete(object sender, SupportAccountAskEvents e);
    public class SupportAccountAskDialog
    {


        public event DelegateSupportAccountAskComplete OnComplete = null;
        Popup _popup;
        public Popup Dialog { get { return _popup; } }
        public TextBox _id = new TextBox();
        public SupportAccountAskDialog()
        {
            _popup = new Popup();
            Border border_root = new Border();
            border_root.Margin = new Thickness(0, 0, 0, 0);
            //border_root.CornerRadius = new CornerRadius(8);
            border_root.Width = 0;//// Application.Current.Host.Content.ActualHeight;
            border_root.Height = 400;
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
            name.NameValue = InputScopeNameValue.Number;
            scope.Names.Add(name);
            _id.InputScope = scope;

            //button
            Grid grid_btn = new Grid();
            grid_btn.Margin = new Thickness(0, 50, 0, 0);
            grid_btn.Width = 750;
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
            if (_id.Text.Length != null)
            {
                try
                {
                    UInt64 id = UInt64.Parse(_id.Text);
                    if (OnComplete != null) OnComplete(this, new SupportAccountAskEvents() { AccountId = id });
                }
                catch (Exception ex)
                {
                    if (OnComplete != null) OnComplete(this, new SupportAccountAskEvents() { AccountId = 0 });
                }
            }
            _popup.IsOpen = false;
        }
        
        public void show()
        {
            _popup.IsOpen = true;
            _popup.VerticalOffset = 0;
        }
    }
    #endregion
}
