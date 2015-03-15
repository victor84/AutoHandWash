using System;

namespace Server.Data
{
    public class Group
    {
        public Guid Id { get; set; }
        public string GroupName { get; set; }
        public Guid UserId { get; set; }
    }
}
