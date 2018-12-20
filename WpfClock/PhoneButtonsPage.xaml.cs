using System;
using System.Collections.Generic;
using System.Diagnostics;
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

namespace WpfClock
{
	/// <summary>
	/// PhoneButtonsPage.xaml の相互作用ロジック
	/// </summary>
	public partial class PhoneButtonsPage : Page
	{
		public EventHandler RaiseEnterEvent;
		public PhoneButtonsPage()
		{
			InitializeComponent();
		}
		public string Text
		{
			get
			{
				return Data.Text;
			}
			set
			{
				Data.Text = value;
			}
		}
		public void SetDefFocus()
		{
			Data.Focus();
		}
		private void Button_Click( object sender, RoutedEventArgs e )
		{
			Data.Text += ((Button)sender).Content.ToString();
		}
		private void EnterButton_Click( object sender, RoutedEventArgs e )
		{
			var handler = RaiseEnterEvent;
			if( handler != null )
			{
				handler( this, EventArgs.Empty );
			}
		}

		private void Page_SizeChanged( object sender, SizeChangedEventArgs e )
		{
			var width = grid.ColumnDefinitions[Grid.GetColumn( EnterButton )].ActualWidth;
			var height = grid.RowDefinitions[Grid.GetRow( EnterButton )].ActualHeight;
			Trace.WriteLine( $"Grid.Width={width}" );
			Trace.WriteLine( $"Grid.Height={height}" );
			Trace.WriteLine( $"EnterButton.ActualWidth={EnterButton.ActualWidth}" );
			Trace.WriteLine( $"EnterButton.ActualHeight={EnterButton.ActualHeight}" );
			if( width > height )
			{
				EnterButton.Height = EnterButton.Width = height-2;
			}
			else
			{
				EnterButton.Height = EnterButton.Width = width-2;
			}
		}
	}
}
