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

        public static List<Group> GetGroups()
        {
            List<Group> groups = null;
            try
            {
                using (var db = new DataConnection())
                {
                    var query = db.GetTable<Group>();
                    groups = query.ToList();
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Group -> GetGroups: {0}", ex);
            }
            return groups;
        }

        public static List<Group> GetGroups(IEnumerable<Guid> ids)
        {
            List<Group> groups = null;
            try
            {
                using (var db = new DataConnection())
                {
                    var query = db.GetTable<Group>().Where(x => ids.Contains(x.Id));
                    groups = query.ToList();
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Group -> GetGroups: {0}", ex);
            }
            return groups;
        }

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
                Console.WriteLine("Group -> GetGroupByName: {0}", ex);
            }
            return group;
        }

        public static Group GetGroupById(Guid id)
        {
            Group group = null;
            try
            {
                using (var db = new DataConnection())
                {
                    group = db.GetTable<Group>().Where(x => x.Id == id).FirstOrDefault();
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Group -> GetGroupById: {0}", ex);
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
