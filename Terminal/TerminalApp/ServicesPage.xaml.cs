using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading;
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
        private IEnumerable<LogicWrapper.tag_service_info> _services_info;

        private TextBlock _widestTextBlock;

        private LogicWrapper.e_state_id _currentState;
        private LogicWrapper.e_state_id _previousState;

        private String _currentServiceName;

        private String _stopServiceName;

        private LogicWrapper.e_service_id _currentService;

        public ServicesPage()
        {

            InitializeComponent();
        }

        public void OnTimeAndMoneyChanged(UInt16 time, UInt16 money)
        {
            this.Dispatcher.BeginInvoke((System.Threading.ThreadStart)delegate()
            {
                TimeSpan timeSpan = TimeSpan.FromSeconds(time);
                RestOfMoneyTextBlock.Text = String.Format("{0}", ((Double)(money / 100.0)).ToString("F"));
            });
        }

        public void OnServiceChanged(LogicWrapper.e_service_id service_id, String service_name)
        {
            _currentServiceName = service_name;
            _currentService = service_id;

            if ((LogicWrapper.e_state_id.paid_idle == _currentState) &&
                (LogicWrapper.e_service_id.stop == service_id))
            {
                return;
            }

            if (LogicWrapper.e_service_id.stop == service_id)
            {
                _stopServiceName = service_name;
            }

        }


        public void OnStateChanged(LogicWrapper.e_state_id state_id)
        {
            _currentState = state_id;

            if (LogicWrapper.e_state_id.advertising_idle == state_id)
            {
                OnServiceChanged(LogicWrapper.e_service_id.stop, "");
            }
            else if (LogicWrapper.e_state_id.executing_service == state_id)
            {

            }
            else if (LogicWrapper.e_state_id.free_idle == state_id)
            {

            }
            else if (LogicWrapper.e_state_id.paid_idle == state_id)
            {

            }
            else if (LogicWrapper.e_state_id.refill_cache == state_id)
            {

            }
            else if (LogicWrapper.e_state_id.settings_work == state_id)
            {

                OnServiceChanged(LogicWrapper.e_service_id.stop, "");
            }
            else if (LogicWrapper.e_state_id.distribution_of_prize == state_id)
            {

            }

            _previousState = state_id;
        }

        public void OnCacheRefilled(UInt16 cache)
        {
            this.Dispatcher.BeginInvoke((System.Threading.ThreadStart)delegate()
            {
                RestOfMoneyTextBlock.Text = String.Format("{0}", ((Double)(cache / 100.0)).ToString("F"));
            });
        }

        public void OnServicesInfoReaded(IEnumerable<LogicWrapper.tag_service_info> collection)
        {
            this.Dispatcher.BeginInvoke((System.Threading.ThreadStart)delegate()
            {
                UpdateServicesNames(collection);
            });
        }

        private void UpdateServicesNames(IEnumerable<LogicWrapper.tag_service_info> collection)
        {
            _services_info = collection;

            FillServicesInfo();

            int maxStrLength = 0;
            String maxString = null;

            for (byte i = 0; i < 8; ++i)
            {
                String str = GetServiceInfo(i);
                int length = str.Length;
                if (length > maxStrLength)
                {
                    maxStrLength = length;
                    maxString = str;
                    break;
                }
            }

            _widestTextBlock = GetTextServiceBlock(maxString);

            ResizeServicesInfo();
        }

        private void FillServicesInfo()
        {
            Service1TextBlock.Text = GetServiceInfo(0);
            Service2TextBlock.Text = GetServiceInfo(1);
            Service3TextBlock.Text = GetServiceInfo(2);
            Service4TextBlock.Text = GetServiceInfo(3);
            Service5TextBlock.Text = GetServiceInfo(4);
            Service6TextBlock.Text = GetServiceInfo(5);
            Service7TextBlock.Text = GetServiceInfo(6);
        }

        void ResizeServicesInfo()
        {
            if (null == _widestTextBlock)
                return;

            Double rowWidth = ServicesGrid.ActualWidth;
            Double rowHeight = ServicesGrid.RowDefinitions[0].ActualHeight;

            Size textSize = MeasureString(_widestTextBlock.Text, _widestTextBlock);


            Double newFontSizeByWidth = 0.0;
            Double newFonSizeByHeigth = 0.0;
            Double newFontSize = 0.0;

            newFonSizeByHeigth = (_widestTextBlock.FontSize * (rowHeight / textSize.Height)) - 0.5;
            newFontSizeByWidth = (_widestTextBlock.FontSize * (rowWidth / textSize.Width)) - 0.5;

            newFontSize = newFonSizeByHeigth < newFontSizeByWidth ? newFonSizeByHeigth : newFontSizeByWidth;

            Service1TextBlock.FontSize = newFontSize;
            Service2TextBlock.FontSize = newFontSize;
            Service3TextBlock.FontSize = newFontSize;
            Service4TextBlock.FontSize = newFontSize;
            Service5TextBlock.FontSize = newFontSize;
            Service6TextBlock.FontSize = newFontSize;
            Service7TextBlock.FontSize = newFontSize;
        }

        private Size MeasureString(String candidate, TextBlock textBlock)
        {
            FormattedText formattedText = new FormattedText(
                candidate,
                CultureInfo.CurrentUICulture,
                FlowDirection.LeftToRight,
                new Typeface(textBlock.FontFamily, textBlock.FontStyle, textBlock.FontWeight, textBlock.FontStretch),
                textBlock.FontSize,
                Brushes.Black);

            return new Size(formattedText.Width, formattedText.Height);
        }

        private TextBlock GetTextServiceBlock(String text)
        {
            if (true == Service1TextBlock.Text.StartsWith(text))
                return Service1TextBlock;
            if (true == Service2TextBlock.Text.StartsWith(text))
                return Service2TextBlock;
            if (true == Service3TextBlock.Text.StartsWith(text))
                return Service3TextBlock;
            if (true == Service4TextBlock.Text.StartsWith(text))
                return Service4TextBlock;
            if (true == Service5TextBlock.Text.StartsWith(text))
                return Service5TextBlock;
            if (true == Service6TextBlock.Text.StartsWith(text))
                return Service6TextBlock;
            if (true == Service7TextBlock.Text.StartsWith(text))
                return Service7TextBlock;

            return null;
        }

        private String GetServiceInfo(byte buttonNumber)
        {
            String result = "";

            foreach(LogicWrapper.tag_service_info si in _services_info)
            {
                if (si.button_number == buttonNumber)
                {
                    result = String.Format("{0}", FormatServiceName(si.service_name), si.cost);
                    break;
                }
            }

            return result;
        }

        private String FormatServiceName(String serviceName)
        {
            String[] words = serviceName.Split(' ');

            if (words.Length < 3)
                return serviceName;

            StringBuilder sb = new StringBuilder();

            for(int i = 0; i < words.Length; ++i)
            {
                sb.Append(words[i]);

                if (1 == i)
                {
                    sb.Append("\r\n");
                }
                else
                {
                    if (i < (words.Length - 1))
                        sb.Append(" ");
                }
            }

            return sb.ToString();
        }

        private void Page_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            ResizeServicesInfo();
        }

        private void Page_Loaded(object sender, RoutedEventArgs e)
        {

        }
    }
}
