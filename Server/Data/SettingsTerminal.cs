using LinqToDB.Mapping;
using System;

namespace Server.Data
{
    [Table("SettingsTerminals")]
    public class SettingsTerminal
    {
        [PrimaryKey, Identity]
        public Guid Id { get; set; }
        [Column]
        public Guid TerminalId { get; set; }
        [Column]
        public byte State { get; set; }
        [Column]
        public uint ImpulseBillAcceptor { get; set; }
        [Column]
        public uint ImpulseCoinAcceptor { get; set; }
        [Column]
        public uint TimeInactivity { get; set; }
        [Column]
        public uint PriceMinuteInactivity { get; set; }
        [Column]
        public uint PauseBeforeShowingAds { get; set; }
        [Column]
        public uint PricePressurizedWater { get; set; }
        [Column]
        public uint PriceNoPressurizedWater { get; set; }
        [Column]
        public uint PriceFoam { get; set; }
        [Column]
        public uint PriceWax { get; set; }
        [Column]
        public uint PriceAgainstOfMidges { get; set; }
        [Column]
        public uint PriceVacuum { get; set; }
        [Column]
        public uint PriceAir { get; set; }
    }
}
