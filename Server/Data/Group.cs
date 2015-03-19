using LinqToDB.Mapping;
using System;

namespace Server.Data
{
    [Table("Groups")]
    public class Group
    {
        [PrimaryKey, Identity]
        public Guid Id { get; set; }
        [Column]
        public string GroupName { get; set; }
        [Column]
        public Guid UserId { get; set; }
    }
}
