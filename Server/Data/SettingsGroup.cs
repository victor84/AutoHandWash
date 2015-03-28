using LinqToDB;
using LinqToDB.Data;
using LinqToDB.Mapping;
using System;
using System.Linq;

namespace Server.Data
{
    [Table("SettingsGroups")]
    public class SettingsGroup
    {
        [PrimaryKey]
        public Guid GroupId { get; set; }
        [Column]
        public byte PercentForPresent { get; set; }
        [Column]
        public uint LowerBoundForRandomSum { get; set; }
        [Column]
        public uint UpperBoundForRandomSum { get; set; }
        [Column]
        public bool HasPresent { get; set; }

        public static SettingsGroup GetSettingsGroupById(Guid Id)
        {
            SettingsGroup settingsGroup = null;
            try
            {
                using (var db = new DataConnection())
                {
                    settingsGroup = db.GetTable<SettingsGroup>().Where(x => x.GroupId == Id).FirstOrDefault();
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("SettingsGroup -> GetSettingsGroupById: {0}", ex);
            }
            return settingsGroup;
        }

        public static bool Insert(SettingsGroup settingsGroup)
        {
            bool result = true;
            try
            {
                using (var db = new DataConnection())
                {
                    db.Insert(settingsGroup);
                }
            }
            catch (Exception ex)
            {
                result = false;
                Console.WriteLine("SettingsGroup -> Insert: {0}", ex);
            }
            return result;
        }

        public static bool Update(SettingsGroup settingsGroup)
        {
            bool result = true;
            try
            {
                using (var db = new DataConnection())
                {
                    db.Update(settingsGroup);
                }
            }
            catch (Exception ex)
            {
                result = false;
                Console.WriteLine("SettingsGroup -> Update: {0}", ex);
            }
            return result;
        }
    }
}
