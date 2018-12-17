using System;
using System.Collections.Generic;
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
	}
}
