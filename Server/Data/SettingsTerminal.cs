using LinqToDB;
using LinqToDB.Data;
using LinqToDB.Mapping;
using System;
using System.Linq;

namespace Server.Data
{
    [Serializable]
    [Table("SettingsTerminals")]
    public class SettingsTerminal
    {
        [PrimaryKey]
        public Guid TerminalId { get; set; }
        [Column]
        public bool DataSent { get; set; }
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
                ServerLogger.Error(string.Format("SettingsTerminal -> GetSettingsTerminalById: {0}", ex.Message));
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
                ServerLogger.Error(string.Format("SettingsTerminal -> Insert: {0}", ex.Message));
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
                ServerLogger.Error(string.Format("SettingsTerminal -> Update: {0}", ex.Message));
            }
            return result;
        }
    }
}
