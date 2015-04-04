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
    /// Логика взаимодействия для ServicesPage.xaml
    /// </summary>
    public partial class ServicesPage : Page
    {
        IEnumerable<LogicWrapper.tag_service_info> _services_info;

        public ServicesPage()
        {
            InitializeComponent();
        }

        public void OnTimeAndMoneyChanged(UInt16 time, UInt16 money)
        {
            RestOfTimeTextBlock.Text = String.Format("Осталось времени: {0} сек.", time);
            RestOfMoneyTextBlock.Text = String.Format("Осталось средств: {0} руб.", ((Double)(money / 100.0)).ToString("F"));
        }

        public void OnServiceChanged(LogicWrapper.e_service_id service_id, String service_name)
        {

        }

        public void OnStateChanged(LogicWrapper.e_state_id state_id)
        {

        }

        public void OnCacheRefilled(UInt16 cache)
        {

        }

        public void OnServicesInfoReaded(IEnumerable<LogicWrapper.tag_service_info> collection)
        {
            _services_info = collection;

            FillServicesInfo();
        }

        private void FillServicesInfo()
        {
            
        }

        private String GetServiceInfo(byte buttonNumber)
        {
            String result = "";

            foreach(LogicWrapper.tag_service_info si in _services_info)
            {
                if (si.button_number == buttonNumber)
                {
                    result = String.Format("{0}\r\n{1} руб.", si.service_name, si.cost);
                }
            }

            return result;
        }

    }
}
