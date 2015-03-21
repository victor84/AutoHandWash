using LinqToDB;
using LinqToDB.Data;
using LinqToDB.Mapping;
using System;
using System.Collections.Generic;
using System.Linq;

namespace Server.Data
{
    [Table("Groups")]
    public class Group
    {
        [Column]
        public Guid Id { get; set; }
        [Column]
        public string GroupName { get; set; }

        public static Group GetGroupByName(string groupName)
        {
            Group group = null;
            try
            {
                using (var db = new DataConnection())
                {
                    group = db.GetTable<Group>().Where(x => x.GroupName == groupName).FirstOrDefault();
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("GetGroupByName: {0}", ex);
            }
            return group;
        }

        public static bool Insert(Group group)
        {
            bool result = true;
            try
            {
                using (var db = new DataConnection())
                {
                    db.Insert(group);
                }
            }
            catch (Exception ex)
            {
                result = false;
                Console.WriteLine("Group -> Insert: {0}", ex);
            }
            return result;
        }
    }
}
