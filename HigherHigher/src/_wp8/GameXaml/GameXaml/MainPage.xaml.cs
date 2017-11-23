using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using Microsoft.Phone.Controls;
using PhoneDirect3DXamlAppComponent;
using System.Diagnostics;
using FacebookUtils;
using Newtonsoft.Json;
using Pine.FacebookUtils;
using Pine.TwitterUtils;
using System.Data.Services.Client;
using System.Collections.ObjectModel;
#if DEBUG
using MockIAPLib;
using Store = MockIAPLib;
using Hammock;
#else
using Windows.ApplicationModel.Store;
#endif
using Microsoft.Phone.Scheduler;
using BackGroundTask;
using Microsoft.Phone.Tasks;
using Hammock;
using Pine.Dialogs;
using System.Text;
using Microsoft.Phone.Net.NetworkInformation;
using Windows.Networking.Sockets;
using System.Net.Http;
using Pine.Actions;
namespace Pine{
    public partial class MainPage : PhoneApplicationPage{
        private Direct3DBackground m_d3dBackground = null;
        PeriodicTask periodicTask;
        string TaskName = "PeriodicAgent";
        public MainPage()
        {
            //Init Component
            InitializeComponent();
            init();
        }
        private void PhoneApplicationPage_OrientationChanged(object sender, OrientationChangedEventArgs e)
        {
            AppObject.OrientationChanged(e.Orientation);
            int ori = 0;
            switch (e.Orientation)
            {
                case PageOrientation.LandscapeLeft: ori = 1; break;
                case PageOrientation.LandscapeRight: ori = 2; break;
                case PageOrientation.PortraitUp: ori = 3; break;
                case PageOrientation.PortraitDown: ori = 4; break;
            }

            if (m_d3dBackground != null) m_d3dBackground.OnOrientationChange(ori);
        }
    }
}