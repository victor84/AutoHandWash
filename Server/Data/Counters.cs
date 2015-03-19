using LinqToDB.Mapping;
using System;

namespace Server.Data
{
    [Table("Counters")]
    public class Counters
    {
        [PrimaryKey, Identity]
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
        public uint ColdWater { get; set; }
        [Column]
        public uint HotWater { get; set; }
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
    }
}
