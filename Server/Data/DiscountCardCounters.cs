using System;
using LinqToDB;
using LinqToDB.Data;
using LinqToDB.Mapping;
using System.Linq;

namespace Server.Data
{
    [Table("DiscountCardCounters")]
    public class DiscountCardCounters
    {
        [Column]
        public Guid Id { get; set; }
        [Column]
        public Guid TerminalId { get; set; }
        [Column]
        public DateTime DateTimeServerEvent { get; set; }
        [Column]
        public byte Status { get; set; }

        public static bool Insert(DiscountCardCounters discountCardCounters)
        {
            bool result = true;
            try
            {
                using (var db = new DataConnection())
                {
                    db.Insert(discountCardCounters);
                }
            }
            catch (Exception ex)
            {
                result = false;
                ServerLogger.Error(string.Format("DiscountCardCounters -> Insert: {0}", ex.Message));
            }
            return result;
        }

        public static int GetSumDiscountCardCountersByTerminal(Guid id)
        {
            int result = 0;
            try
            {
                using (var db = new DataConnection())
                {
                    var table = db.GetTable<DiscountCardCounters>();
                    result = table.Where(x => x.TerminalId == id && x.Status == 0).Count();
                }
            }
            catch (Exception ex)
            {
                ServerLogger.Error(string.Format("DiscountCardCounters -> GetSumDiscountCardCountersByTerminal: {0}", ex.Message));
            }
            return result;
        }
    }
}
