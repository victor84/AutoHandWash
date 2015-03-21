using LinqToDB;
using LinqToDB.Data;
using LinqToDB.Mapping;
using System;
using System.Linq;
using System.Collections.Generic;

namespace Server.Data
{
    [Table("SettingsTerminals")]
    public class SettingsTerminal
    {
        [PrimaryKey]
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
        [Column]
        public uint PriceOsmose { get; set; }

        public static SettingsTerminal GetSettingsTerminalById(Guid id)
        {
            SettingsTerminal settingsTerminal = null;
            try
            {
                using (var db = new DataConnection())
                {
                    settingsTerminal = db.GetTable<SettingsTerminal>().Where(x => x.TerminalId == id).FirstOrDefault();
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("GetSettingsTerminalById: {0}", ex);
            }
            return settingsTerminal;
        }

        public static bool Insert(SettingsTerminal settingsTerminal)
        {
            bool result = true;
            try
            {
                using (var db = new DataConnection())
                {
                    db.Insert(settingsTerminal);
                }
            }
            catch (Exception ex)
            {
                result = false;
                Console.WriteLine("SettingsTerminal -> Insert: {0}", ex);
            }
            return result;
        }

        public static bool Update(SettingsTerminal settingsTerminal)
        {
            bool result = true;
            try
            {
                using (var db = new DataConnection())
                {
                    db.Update(settingsTerminal);
                }
            }
            catch (Exception ex)
            {
                result = false;
                Console.WriteLine("SettingsTerminal -> Update: {0}", ex);
            }
            return result;
        }
    }
}
