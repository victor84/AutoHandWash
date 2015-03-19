using LinqToDB.Mapping;
using System;

namespace Server.Data
{
    [Table("Terminals")]
    public class Terminal
    {
        [PrimaryKey, Identity]
        public Guid Id { get; set; }
        [Column]
        public string TerminalName { get; set; }
        [Column]
        public Guid GroupId { get; set; }
    }
}
