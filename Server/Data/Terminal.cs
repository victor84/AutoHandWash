using System;

namespace Server.Data
{
    public class Terminal
    {
        public Guid Id { get; set; }
        public string TerminalName { get; set; }
        public Guid GroupId { get; set; }
        public DateTime DateTimeServerEvent { get; set; }
        public DateTime DateTimeTerminalEvent { get; set; }
        public uint CommonInput { get; set; }
        public uint Balance { get; set; }
        public uint ColdWater { get; set; }
        public uint HotWater { get; set; }
        public uint Foam { get; set; }
        public uint Wax { get; set; }
        public uint AgainstOfMidges { get; set; }
        public uint Osmose { get; set; }
        public uint Vacuum { get; set; }
    }
}
