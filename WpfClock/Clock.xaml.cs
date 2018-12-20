using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Interop;
using System.Windows.Media.Animation;
using System.Windows.Threading;


namespace WpfClock
{
	/// <summary>
	/// Clock.xaml の相互作用ロジック
	/// </summary>
	public partial class Clock : Page
	{
		const int WS_CHILD = 0x40000000;
		const int WS_VISIBLE = 0x10000000;

		private DispatcherTimer _dayTimer;
		public Clock()
		{
			InitializeComponent();
		}

		private void OnDayChange( object sender, EventArgs e )
		{
			var now = DateTime.Now;
			DataContext = now.Day.ToString();
			_dayTimer.Interval = new TimeSpan( 1, 0, 0, 0 );
		}
		private void Page_Loaded( object sender, RoutedEventArgs e )
		{
			var now = DateTime.Now;
			DataContext = now.Day.ToString();

			// then set up a timer to fire at the start of tomorrow, so that we can update
			// the datacontext
			_dayTimer = new DispatcherTimer { Interval = new TimeSpan( 1, 0, 0, 0 ) - now.TimeOfDay };
			_dayTimer.Tick += OnDayChange;
			_dayTimer.Start();

			// finally, seek the timeline, which assumes a beginning at midnight, to the appropriate
			// offset
			var sb = (Storyboard)PodClock.FindResource( "sb" );
			sb.Begin( PodClock, HandoffBehavior.SnapshotAndReplace, true );
			sb.Seek( PodClock, now.TimeOfDay, TimeSeekOrigin.BeginTime );
		}
	}
}
