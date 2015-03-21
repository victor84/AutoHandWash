using LinqToDB;
using LinqToDB.Data;
using LinqToDB.Mapping;
using System;

namespace Server.Data
{
    [Table("SettingsGroups")]
    public class SettingsGroup
    {
        [Column]
        public Guid GroupId { get; set; }
        [Column]
        public byte PercentForPresent { get; set; }
        [Column]
        public uint LowerBoundForRandomSum { get; set; }
        [Column]
        public uint UpperBoundForRandomSum { get; set; }
        [Column]
        public bool HasPresent { get; set; }
    }
}
