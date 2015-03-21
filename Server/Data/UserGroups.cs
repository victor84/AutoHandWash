using LinqToDB;
using LinqToDB.Data;
using LinqToDB.Mapping;
using System;

namespace Server.Data
{
    [Table("UserGroups")]
    public class UserGroups
    {
        [Column]
        public Guid GroupId { get; set; }
        [Column]
        public Guid UserId { get; set; }

        public static bool Insert(UserGroups userGroups)
        {
            bool result = true;
            try
            {
                using (var db = new DataConnection())
                {
                    db.Insert(userGroups);
                }
            }
            catch (Exception ex)
            {
                result = false;
                Console.WriteLine("UserGroups -> Insert: {0}", ex);
            }
            return result;
        }
    }
}
