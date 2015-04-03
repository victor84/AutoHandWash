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

namespace TerminalApp
{
    /// <summary>
    /// Логика взаимодействия для MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        LogicWrapper.Logic _logic;

        public MainWindow()
        {
            InitializeComponent();

            _logic = new LogicWrapper.Logic();

        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            _logic.SetDelegates(OnTimeAndMoneyChanged, OnServiceChanged);
        }

        private void OnTimeAndMoneyChanged(UInt16 time, UInt16 money)
        {

        }

        private void OnServiceChanged(LogicWrapper.e_service_id service_id, String service_name)
        {

        }

        private void Window_Closed(object sender, EventArgs e)
        {
            _logic.Stop();
            _logic.Dispose();
        }

        private void Start_Click(object sender, RoutedEventArgs e)
        {
            _logic.Start();
        }

    }
}
