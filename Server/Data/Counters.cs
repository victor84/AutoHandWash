using LinqToDB;
using LinqToDB.Data;
using LinqToDB.Mapping;
using System;
using System.Linq;
using System.Collections.Generic;

namespace Server.Data
{
    [Table("Counters")]
    public class Counters
    {
        [Column]
        public Guid Id { get; set; }
        [Column]
        public Guid TerminalId { get; set; }
        [Column]
        public DateTime DateTimeServerEvent { get; set; }
        [Column]
        public DateTime DateTimeTerminalEvent { get; set; }
        [Column]
        public byte State { get; set; }
        [Column]
        public uint CommonInput { get; set; }
        [Column]
        public uint Balance { get; set; }
        [Column]
        public uint PressurizedWater { get; set; }
        [Column]
        public uint NoPressurizedWater { get; set; }
        [Column]
        public uint Foam { get; set; }
        [Column]
        public uint Wax { get; set; }
        [Column]
        public uint AgainstOfMidges { get; set; }
        [Column]
        public uint Osmose { get; set; }
        [Column]
        public uint Vacuum { get; set; }
        [Column]
        public uint Air { get; set; }

        public static bool Insert(Counters counters)
        {
            bool result = true;
            try
            {
                using (var db = new DataConnection())
                {
                    db.Insert(counters);
                }
            }
            catch (Exception ex)
            {
                result = false;
                Console.WriteLine("Counters -> Insert: {0}", ex);
            }
            return result;
        }

        //public static List<Counters> GetCountersByTerminal(IEnumerable<Guid> ids)
        //{
        //    List<Counters> counters = null;
        //    try
        //    {
        //        using (var db = new DataConnection())
        //        {
        //            var query = db.GetTable<Counters>().
        //                Where(x => ids.Contains(x.TerminalId)).
        //                GroupBy(x => x.TerminalId).
        //                Select(x => x.OrderByDescending(y => y.DateTimeServerEvent).First());
                    
        //            counters = query.ToList();
        //        }
        //    }
        //    catch (Exception ex)
        //    {
        //        Console.WriteLine("Counters -> GetCounters: {0}", ex);
        //    }
        //    return counters;
        //}

        public static Counters GetCountersByTerminal(Guid id)
        {
            Counters counters = null;
            try
            {
                using (var db = new DataConnection())
                {
                    counters = db.GetTable<Counters>().
                                Where(x => x.TerminalId == id).
                                OrderByDescending(i => i.DateTimeServerEvent).
                                FirstOrDefault();
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Counters -> GetCounters: {0}", ex);
            }
            return counters;
        }
    }
}
