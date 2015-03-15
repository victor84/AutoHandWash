using System;

namespace Server.Data
{
    public class SettingsGroup
    {
        public Guid Id { get; set; }
        public Guid GroupId { get; set; }
        public byte PercentForPresent { get; set; }
        public uint LowerBoundForRandomSum { get; set; }
        public uint UpperBoundForRandomSum { get; set; }
        public bool HasPresent { get; set; }
    }
}
