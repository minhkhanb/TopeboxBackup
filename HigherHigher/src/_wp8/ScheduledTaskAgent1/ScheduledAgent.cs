using System.Diagnostics;
using System.Windows;
using Microsoft.Phone.Scheduler;
using Microsoft.Phone.Shell;
using System;
using Microsoft.Phone.Notification;
using System.IO;
namespace BackGroundTask
{
    public class ScheduledAgent : ScheduledTaskAgent
    {
        string[] NotifyText = new string[]{
            "You've completed upgrading your soldier. Return to Pocket Army and visit your army?",
	        "You've completed training a new soldier. Return to Pocket Army and put them to fight?",
	        "Abaddon has returned. Go back to Pocket Army and defend him now!",
	        "Your army miss you! Give them a cause worth fighting for!",
	        "Don't forget to invite a friend to ask for help in your battle!",
	        "Win a friend challenge, you will be rewarded by gem. Go back to Pocket Army and challenge your friends now for more gems!",
	        "Come back to Pocket Army for today's reward! Come back each day will give your a better daily reward every day.",
	        "Some people really need your help! Please help them!",
	        "Some people really need your help! Please help them!",
	        "We found the location of The Gem Felix. Catch him now?"
        };
        /// <remarks>
        /// ScheduledAgent constructor, initializes the UnhandledException handler
        /// </remarks>
        static ScheduledAgent()
        {
            // Subscribe to the managed exception handler
            Deployment.Current.Dispatcher.BeginInvoke(delegate
            {
                Application.Current.UnhandledException += UnhandledException;
            });
        }

        /// Code to execute on Unhandled Exceptions
        private static void UnhandledException(object sender, ApplicationUnhandledExceptionEventArgs e)
        {
            if (Debugger.IsAttached)
            {
                // An unhandled exception has occurred; break into the debugger
                Debugger.Break();
            }
        }
        private int readInt32(FileStream fs)
        {
            int value = 0;
            value |= (fs.ReadByte() << 24);
            value |= (fs.ReadByte() << 16);
            value |= (fs.ReadByte() << 8);
            value |= (fs.ReadByte());
            return value;
        }
        private void writeInt32(FileStream fs, int value)
        {
            fs.WriteByte((byte)(value >> 24));
            fs.WriteByte((byte)(value >> 16));
            fs.WriteByte((byte)(value >> 8));
            fs.WriteByte((byte)(value));
        }
        /// <summary>
        /// Agent that runs a scheduled task
        /// </summary>
        /// <param name="task">
        /// The invoked task
        /// </param>
        /// <remarks>
        /// This method is called when a periodic or resource intensive task is invoked
        /// </remarks>
        protected override void OnInvoke(ScheduledTask task)
        {
            DateTime last_call_time = task.LastScheduledTime;
            //TODO: Add code to perform your task in background
            Windows.ApplicationModel.Package package = Windows.ApplicationModel.Package.Current;
			Windows.Storage.StorageFolder installedLocation = package.InstalledLocation;
            string file_output = Path.Combine(installedLocation.Path, "notify.dat");
            Int32[] notify = new Int32[10];
            for (int i = 0; i < 10; i++)
            {
                notify[i] = -1;
            }
            DateTime last_time;
            

            using(FileStream fs = File.Open(file_output, FileMode.Open)){
                int year = readInt32(fs);
                int month = readInt32(fs);
                int day = readInt32(fs);
                int hour = readInt32(fs);
                int minute = readInt32(fs);
                int second = readInt32(fs);
                last_time = new DateTime(year, month, day, hour, minute, second);

                for (int i = 0; i < 10; i++)
                {
                    notify[i] = 0;
                    notify[i] |= (fs.ReadByte() << 24);
                    notify[i] |= (fs.ReadByte() << 16);
                    notify[i] |= (fs.ReadByte() << 8);
                    notify[i] |= (fs.ReadByte());
                }
                fs.Close();
            }
            DateTime timenow = DateTime.Now.ToLocalTime();
            TimeSpan delattime =  timenow - last_time;
            int deltasecond = (int)delattime.TotalSeconds;
            bool is_complete = true;
            for (int i = 0; i < 10; i++)
            {
                if (notify[i] > -1)
                {
                    if (notify[i] < deltasecond)
                    {
                        string toastMessage = NotifyText[i];
                        ShellToast toast = new ShellToast();
                        toast.Title = "Pocket Army";
                        toast.Content = toastMessage;
                        toast.Show();
                    }
                    notify[i] -= deltasecond;
                    is_complete = false;
                }
            }
            if (is_complete)
            {
                NotifyComplete();
                ScheduledActionService.Remove(task.Name);
            }
            else
            {
                using (FileStream fs = File.Open(file_output, FileMode.Open))
                {
                    writeInt32(fs, timenow.Year);
                    writeInt32(fs, timenow.Month);
                    writeInt32(fs, timenow.Day);
                    writeInt32(fs, timenow.Hour);
                    writeInt32(fs, timenow.Minute);
                    writeInt32(fs, timenow.Second);
                    for (int i = 0; i < 10; i++)
                    {
                        writeInt32(fs, notify[i]);
                    }
                    fs.Close();
                }
#if DEBUG
                ScheduledActionService.LaunchForTest(task.Name, TimeSpan.FromSeconds(60));
#endif
                NotifyComplete();
            }
#if DEBUG
            ScheduledActionService.LaunchForTest(task.Name,TimeSpan.FromSeconds(60));
#endif
            /*
            /// Holds the push channel that is created or found.
            HttpNotificationChannel pushChannel;
            // The name of our push channel.
            string channelName = "PocketArmyChannel";
            pushChannel = HttpNotificationChannel.Find(channelName);

            // If the channel was not found, then create a new connection to the push service.
            //if (pushChannel == null)
            //{
                pushChannel = new HttpNotificationChannel(channelName);
                // Register for all the events before attempting to open the channel.
                pushChannel.ChannelUriUpdated += new EventHandler<NotificationChannelUriEventArgs>(PushChannel_ChannelUriUpdated);
                pushChannel.ErrorOccurred += new EventHandler<NotificationChannelErrorEventArgs>(PushChannel_ErrorOccurred);
                pushChannel.Open();
                // Bind this new channel for Tile events.
                pushChannel.BindToShellTile();
            //}
            //else
            //{
            //    // The channel was already open, so just register for all the events.
            //    pushChannel.ChannelUriUpdated += new EventHandler<NotificationChannelUriEventArgs>(PushChannel_ChannelUriUpdated);
            //    pushChannel.ErrorOccurred += new EventHandler<NotificationChannelErrorEventArgs>(PushChannel_ErrorOccurred);
            //    // Display the URI for testing purposes. Normally, the URI would be passed back to your web service at this point.
            //    System.Diagnostics.Debug.WriteLine(pushChannel.ChannelUri.ToString());
            //    MessageBox.Show(String.Format("Channel Uri is {0}",
            //        pushChannel.ChannelUri.ToString()));

            //}
            */
            
        }
    }
}