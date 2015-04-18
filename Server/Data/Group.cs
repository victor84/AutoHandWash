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
                ServerLogger.Error(string.Format("Group -> GetGroups: {0}", ex.Message));
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
                ServerLogger.Error(string.Format("Group -> GetGroups: {0}", ex.Message));
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
                ServerLogger.Error(string.Format("Group -> GetGroupByName: {0}", ex.Message));
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
                ServerLogger.Error(string.Format("Group -> GetGroupById: {0}", ex.Message));
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
                ServerLogger.Error(string.Format("Group -> Insert: {0}", ex.Message));
            }
            return result;
        }
    }
}
