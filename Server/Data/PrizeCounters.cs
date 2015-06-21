using LinqToDB;
using LinqToDB.Data;
using LinqToDB.Mapping;
using System;
using System.Linq;

namespace Server.Data
{
    [Table("PrizeCounters")]
    public class PrizeCounters
    {
        [Column]
        public Guid Id { get; set; }
        [Column]
        public Guid TerminalId { get; set; }
        [Column]
        public DateTime DateTimeServerEvent { get; set; }
        [Column]
        public byte Status { get; set; }
        [Column]
        public uint Size { get; set; }

        public static bool Insert(PrizeCounters prizeCounters)
        {
            bool result = true;
            try
            {
                using (var db = new DataConnection())
                {
                    db.Insert(prizeCounters);
                }
            }
            catch (Exception ex)
            {
                result = false;
                ServerLogger.Error(string.Format("PrizeCounters -> Insert: {0}", ex.Message));
            }
            return result;
        }

        public static long GetSumPrizeCountersByTerminal(Guid id)
        {
            long result = 0;
            try
            {
                using (var db = new DataConnection())
                {
                    var table = db.GetTable<PrizeCounters>();
                    var query = table.Where(x => x.TerminalId == id).ToList();
                    result = query.Sum(x => x.Size);
                }
            }
            catch (Exception ex)
            {
                ServerLogger.Error(string.Format("PrizeCounters -> GetSumPrizeCountersByTerminal: {0}", ex.Message));
            }
            return result;
        }
    }
}
