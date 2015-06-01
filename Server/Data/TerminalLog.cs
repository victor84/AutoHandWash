using LinqToDB;
using LinqToDB.Data;
using LinqToDB.Mapping;
using System;
using System.Collections.Generic;
using System.Linq;

namespace Server.Data
{
    [Table("TerminalLogs")]
    public class TerminalLog
    {
        [Column]
        public Guid Id { get; set; }
        [Column]
        public Guid TerminalId { get; set; }
        [Column]
        public byte MessageType { get; set; }
        [Column]
        public DateTime DateTimeTerminal { get; set; }
        [Column]
        public string Message { get; set; }

        public static List<TerminalLog> GetLogsByTerminal(Guid id)
        {
            List<TerminalLog> logs = null;
            try
            {
                using (var db = new DataConnection())
                {
                    var query = db.GetTable<TerminalLog>().Where(x => x.TerminalId == id);
                    logs = query.ToList();
                }
            }
            catch (Exception ex)
            {
                ServerLogger.Error(string.Format("TerminalLog -> GetLogsByTerminal: {0}", ex.Message));
            }
            return logs;
        }

        public static bool Insert(TerminalLog terminalLog)
        {
            bool result = true;
            try
            {
                using (var db = new DataConnection())
                {
                    db.Insert(terminalLog);
                }
            }
            catch (Exception ex)
            {
                result = false;
                ServerLogger.Error(string.Format("TerminalLog -> Insert: {0}", ex.Message));
            }
            return result;
        }
    }
}
