using System;
using System.Collections.Generic;
using System.Globalization;
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
        private IEnumerable<LogicWrapper.tag_service_info> _services_info;

        private TextBlock _widestTextBlock;

        private LogicWrapper.e_state_id _currentState;

        public ServicesPage()
        {
            InitializeComponent();
        }

        public void OnTimeAndMoneyChanged(UInt16 time, UInt16 money)
        {
            this.Dispatcher.BeginInvoke((System.Threading.ThreadStart)delegate()
            {
                RestOfTimeTextBlock.Text = String.Format("{0} сек.", time);
                RestOfMoneyTextBlock.Text = String.Format("{0} руб.", ((Double)(money / 100.0)).ToString("F"));
            });
        }

        public void OnServiceChanged(LogicWrapper.e_service_id service_id, String service_name)
        {
            if ((LogicWrapper.e_state_id.paid_idle == _currentState) &&
                (LogicWrapper.e_service_id.stop == service_id))
            {
                return;
            }

            SetServiceNameTextBlockText(service_name);
        }

        private void SetServiceNameTextBlockText(String text)
        {
            this.Dispatcher.BeginInvoke((System.Threading.ThreadStart)delegate()
            {
                CurrentServiceNameTextBlock.Text = text;
            });
        }

        private void ShowHint(String text)
        {
            this.Dispatcher.BeginInvoke((System.Threading.ThreadStart)delegate()
            {
                HintTextBlock.Text = text;
            });
        }

        public void OnStateChanged(LogicWrapper.e_state_id state_id)
        {
            _currentState = state_id;
            String stateText = "";
            if (LogicWrapper.e_state_id.advertising_idle == state_id)
            {
                ShowHint("Внесите деньги");
                OnServiceChanged(LogicWrapper.e_service_id.stop, "");
                stateText = "Реклама";
            }
            else if (LogicWrapper.e_state_id.executing_service == state_id)
            {
                stateText = "Выполнение услуги...";
                ShowHint("");
            }
            else if (LogicWrapper.e_state_id.free_idle == state_id)
            {
                stateText = "Бесплатный простой";
                ShowHint("");
            }
            else if (LogicWrapper.e_state_id.paid_idle == state_id)
            {
                stateText = "Платный простой";
                ShowHint("");
                SetServiceNameTextBlockText("Платный простой");
            }
            else if (LogicWrapper.e_state_id.refill_cache == state_id)
            {
                stateText = "Пополнение счёта";
                ShowHint("Выберите услугу");
            }
            else if (LogicWrapper.e_state_id.settings_work == state_id)
            {
                stateText = "Работа с настройками";
                OnServiceChanged(LogicWrapper.e_service_id.stop, "");
                ShowHint("Подождите...");
            }
            else if (LogicWrapper.e_state_id.distribution_of_prize == state_id)
            {
                stateText = "Выдача приза";
                ShowHint("Приз");
            }
            
            this.Dispatcher.BeginInvoke((System.Threading.ThreadStart)delegate()
            {
                LogicStateTextBlock.Text = stateText;
            });
        }

        public void OnCacheRefilled(UInt16 cache)
        {
            this.Dispatcher.BeginInvoke((System.Threading.ThreadStart)delegate()
            {
                RestOfMoneyTextBlock.Text = String.Format("{0} руб.", ((Double)(cache / 100.0)).ToString("F"));
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

            Size textSize = MeasureString(_widestTextBlock.Text, _widestTextBlock);
            Double newFontSizeDouble = (_widestTextBlock.FontSize * (rowWidth / textSize.Width)) - 0.5;

            Service1TextBlock.FontSize = newFontSizeDouble;
            Service2TextBlock.FontSize = newFontSizeDouble;
            Service3TextBlock.FontSize = newFontSizeDouble;
            Service4TextBlock.FontSize = newFontSizeDouble;
            Service5TextBlock.FontSize = newFontSizeDouble;
            Service6TextBlock.FontSize = newFontSizeDouble;
            Service7TextBlock.FontSize = newFontSizeDouble;
        }

        private Size MeasureString(string candidate, TextBlock textBlock)
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
                    result = String.Format("{0}: {1} руб.", si.service_name, si.cost);
                    break;
                }
            }

            return result;
        }

        private void Page_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            ResizeServicesInfo();
        }

    }
}
