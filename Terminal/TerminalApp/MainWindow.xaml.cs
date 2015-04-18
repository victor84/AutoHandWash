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
            ServicesPage servicePage = (ServicesPage)ServicePageFrame.Content;

            _logic.SetDelegate(servicePage.OnTimeAndMoneyChanged);
            _logic.SetDelegate(servicePage.OnServiceChanged);

            LogicWrapper.OnStateChangedDelegate stateChangedDelegate = new LogicWrapper.OnStateChangedDelegate(OnStateChanged);
            stateChangedDelegate += servicePage.OnStateChanged;
            _logic.SetDelegate(stateChangedDelegate);

            _logic.SetDelegate(servicePage.OnCacheRefilled);
            _logic.SetDelegate(servicePage.OnServicesInfoReaded);

            _logic.SetDelegate(servicePage.OnDistributionPrizeDelegate);
            _logic.SetDelegate(servicePage.OnEmptyHopper);

            _logic.Start();
        }

        private void OnStateChanged(LogicWrapper.e_state_id state_id)
        {

        }

        private void Window_Closed(object sender, EventArgs e)
        {
            _logic.Stop();
            _logic.Dispose();
        }
    }
}
