using Server.Data;
using System.Collections.Generic;
using System.Linq;

namespace Server.Models
{
    public enum MainErrors
    {
        NotGroups,
        ErrorTerminals,
        ErrorTerminalsLogs,
        ErrorFillBalance,
        ErrorChangeStatus,
        ErrorPrizeFund,
        ErrorPrizeGroup,
    };

    public class MainError
    {
        public MainErrors Type { get; set; }
        public string Message
        {
            get
            {
                string result = string.Empty;
                switch (Type)
                {
                    case MainErrors.NotGroups:
                        result = "Нет групп, привязанных к пользователю";
                        break;
                    case MainErrors.ErrorTerminals:
                        result = "Не удалось прочитать счетчики терминалов";
                        break;
                    case MainErrors.ErrorTerminalsLogs:
                        result = "Не удалось прочитать сообщения терминалов";
                        break;
                    case MainErrors.ErrorFillBalance:
                        result = "Ошибка при пополнении баланса терминала";
                        break;
                    case MainErrors.ErrorChangeStatus:
                        result = "Не удалось изменить статус терминала";
                        break;
                    case MainErrors.ErrorPrizeFund:
                        result = "Не удалось прочитать призовой фонд";
                        break;
                    case MainErrors.ErrorPrizeGroup:
                        result = "Все призовые суммы разыграны, новый призовой фонд будет сформирован при пополнении баланса";
                        break;
                    default:
                        break;
                }
                return result;
            }
        }
    }

    public class GroupInfo
    {
        private int count = 10; 
        public List<TerminalCounters> TerminalCounters { get; set; }
        public List<TerminalLogs> ViewTerminalLogs
        {
            get
            {
                return TerminalLogs.OrderByDescending(x => x.TerminalLog.DateTimeTerminal).Take(count).ToList();
            }
        }
        public List<TerminalLogs> TerminalLogs;

        public GroupInfo()
        {
            TerminalCounters = new List<TerminalCounters>();
            TerminalLogs = new List<TerminalLogs>();
        }
    }

    public class TerminalLogs
    {
        public string TerminalName { get; set; }
        public string GroupName { get; set; }
        public TerminalLog TerminalLog { get; set; }

        public TerminalLogs()
        {
            TerminalLog = new TerminalLog();
        }
    }

    public class TerminalCounters
    {
        public string TerminalName { get; set; }
        public double Balance
        {
            get
            {
                double result = Counters.Balance / 100;
                return result;
            }
        }
        public Counters Counters { get; set; }
        public long SumPrizeCounters { get; set; }
        public int SumDiscountCardCounters { get; set; }

        public TerminalCounters()
        {
            Counters = new Counters();
        }
    }

    public class MainPageModel
    {
        public Dictionary<Group, GroupInfo> DictionaryGroupInfo { get; set; }
        public List<TerminalLogs> TerminalLogs { get; set; }
        public MainError MainError { get; set; }
        public MainPageModel()
        {
            DictionaryGroupInfo = new Dictionary<Group, GroupInfo>();
            TerminalLogs = new List<TerminalLogs>();
        }
    }
}
