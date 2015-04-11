using Server.Data;
using System.Collections.Generic;
namespace Server.Models
{
    public enum MainErrors
    {
        NotGroups,
        ErrorTerminals,
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
                    default:
                        break;
                }
                return result;
            }
        }
    }
    
    public class TerminalCounters
    {
        public string TerminalName { get; set; }
        public Counters Counters { get; set; }

        public TerminalCounters()
        {
            Counters = new Counters();
        }
    }
    
    public class MainPageModel
    {
        public Dictionary<Group, List<TerminalCounters>> DictionaryTerminalCounters { get; set; }
        public MainError MainError { get; set; }
        public MainPageModel()
        {
            DictionaryTerminalCounters = new Dictionary<Group, List<TerminalCounters>>();
        }
    }
}
