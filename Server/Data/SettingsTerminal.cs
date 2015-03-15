using System;

namespace Server.Data
{
    public class SettingsTerminal
    {
        public Guid Id { get; set; }
        public Guid TerminalId { get; set; }
        public uint ImpulseBillAcceptor { get; set; }
        public uint ImpulseCoinAcceptor { get; set; }
        public uint TimeInactivity { get; set; }
        public uint PriceMinuteInactivity { get; set; }
        public uint PauseBeforeShowingAds { get; set; }
        public bool StatusWork { get; set; }
        public uint PricePressurizedWater { get; set; }
        public uint PriceNoPressurizedWater { get; set; }
        public uint PriceFoam { get; set; }
        public uint PriceWax { get; set; }
        public uint PriceAgainstOfMidges { get; set; }
        public uint PriceVacuum { get; set; }
        public uint PriceAir { get; set; }
    }
}
