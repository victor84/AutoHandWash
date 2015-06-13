using System;
using System.Collections.Generic;
using System.IO;
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
    /// Логика взаимодействия для PrizePage.xaml
    /// </summary>
    public partial class PrizePage : Page
    {
        private ShowPageDelegate _showPage;

        private MediaPlayer _audioPlayer;

        private bool _isPlaying;

        public PrizePage()
        {
            InitializeComponent();

            _audioPlayer = new MediaPlayer();

            String currentFolder = Directory.GetCurrentDirectory();

            String[] files = Directory.GetFiles(currentFolder, "prize_win.*", SearchOption.AllDirectories);

            if ((null != files) && (0 != files.Length))
            {
                _audioPlayer.Open(new Uri(files[0], UriKind.Absolute));
                _audioPlayer.MediaEnded += _audioPlayer_MediaEnded;
            }

            _isPlaying = false;
        }

        void _audioPlayer_MediaEnded(object sender, EventArgs e)
        {
            if (true == _isPlaying)
                _audioPlayer.Play();
        }

        public void OnDistributionPrize(Int16 prize_size, Byte balance)
        {
            this.Dispatcher.BeginInvoke((System.Threading.ThreadStart)delegate()
            {
                PrizeSizeTextBlock.Text = prize_size.ToString();
                CoinBalanceTextBlock.Text = String.Format("Осталось получить {0} монет", balance);

                if (0 == balance)
                {
                    StopSound();

                    if (null != _showPage)
                        _showPage(0);
                }
                else
                {
                    PlaySound();
                }
            });
        }

        private void PlaySound()
        {
            if (false == _isPlaying)
            {
                if (true == _audioPlayer.HasAudio)
                {
                    _isPlaying = true;
                    _audioPlayer.Play();
                }
            }
        }

        private void StopSound()
        {
            if (true == _isPlaying)
            {
                _isPlaying = false;
                _audioPlayer.Stop();
            }
        }

        public void OnEmptyHopper()
        {
            this.Dispatcher.BeginInvoke((System.Threading.ThreadStart)delegate()
            {
                CoinBalanceTextBlock.Text = "Монеты закончились. Обратитесь к оператору.";
            });
        }

        public void SetShowPageFn(ShowPageDelegate fn)
        {
            _showPage = fn;
        }
    }
}
