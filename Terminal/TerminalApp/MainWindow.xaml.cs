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
using System.IO;

namespace TerminalApp
{
    /// <summary>
    /// Вызывать для смены отображаемой страницы
    /// </summary>
    /// <param name="number"></param>
    public delegate void ShowPageDelegate(Int32 number);

    /// <summary>
    /// Логика взаимодействия для MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        /// <summary>
        /// Экземпляр логики
        /// </summary>
        LogicWrapper.Logic _logic;

        /// <summary>
        /// Текущее состояние
        /// </summary>
        LogicWrapper.e_state_id _currentState;

        /// <summary>
        /// Предыдущее состояние
        /// </summary>
        LogicWrapper.e_state_id _previousState;

        /// <summary>
        /// Предыдущее состояние терминала
        /// </summary>
        LogicWrapper.e_terminal_state _previousTerminalState;

        /// <summary>
        /// Текущее состояние терминала
        /// </summary>
        LogicWrapper.e_terminal_state _currentTerminalState;

        public MainWindow()
        {
            InitializeComponent();

            _logic = new LogicWrapper.Logic();
        }

        public void ShowPage(Int32 number)
        {
            this.Dispatcher.BeginInvoke((System.Threading.ThreadStart)delegate()
            {
                MainTabControl.SelectedIndex = number;
            });
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            String currentFolder = Directory.GetCurrentDirectory();

            String[] files = Directory.GetFiles(currentFolder, "advertising.*", SearchOption.AllDirectories);

            if ((null != files) && (0 != files.Length))
            {
                VideoPlayer.Source = new Uri(files[0], UriKind.Absolute);
            }

            ServicesPage servicePage = (ServicesPage)ServicePageFrame.Content;
            PrizePage prizePage = (PrizePage)PrizePageFrame.Content;
            prizePage.SetShowPageFn(ShowPage);

            _logic.SetDelegate(servicePage.OnTimeAndMoneyChanged);
            _logic.SetDelegate(servicePage.OnServiceChanged);

            LogicWrapper.OnStateChangedDelegate stateChangedDelegate = new LogicWrapper.OnStateChangedDelegate(OnStateChanged);
            stateChangedDelegate += servicePage.OnStateChanged;
            _logic.SetDelegate(stateChangedDelegate);

            _logic.SetDelegate(servicePage.OnCacheRefilled);
            _logic.SetDelegate(servicePage.OnServicesInfoReaded);

            _logic.SetDelegate(prizePage.OnDistributionPrize);
            _logic.SetDelegate((LogicWrapper.OnEmptyHopperDelegate)prizePage.OnEmptyHopper);

            _logic.SetDelegate((LogicWrapper.OnShowAdvertisingDelegate)OnShowAdvertising);

            _logic.SetDelegate(OnTerminalStateChanged);

            _logic.Start();
        }

        private void OnTerminalStateChanged(LogicWrapper.e_terminal_state state)
        {
            _currentTerminalState = state;

            if (_currentTerminalState != LogicWrapper.e_terminal_state.work)
            {
                ShowPage(3);
            }
            else
            {
                if (LogicWrapper.e_terminal_state.work != _previousTerminalState)
                {
                    ShowPage(0);
                }
            }

            _previousTerminalState = _currentTerminalState;
        }

        private void OnStateChanged(LogicWrapper.e_state_id state_id)
        {
            _currentState = state_id;

            StateChangingWork();

            _previousState = _currentState;
        }

        private void Window_Closed(object sender, EventArgs e)
        {
            VideoPlayer.Close();

            _logic.Stop();
            _logic.Dispose();
        }

        private void StateChangingWork()
        {
            if (LogicWrapper.e_state_id.distribution_of_prize == _currentState)
            {
                ShowPage(2);
            }
            else if (LogicWrapper.e_state_id.advertising_idle == _previousState)
            {
                ShowPage(0);
            }
        }

        private void OnShowAdvertising()
        {
            if (_currentTerminalState != LogicWrapper.e_terminal_state.work)
                return;

            this.Dispatcher.BeginInvoke((System.Threading.ThreadStart)delegate()
            {
                MainTabControl.SelectedIndex = 1;
                VideoPlayer.Play();
            });
        }
    }
}
