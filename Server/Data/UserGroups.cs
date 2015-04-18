using LinqToDB;
using LinqToDB.Data;
using LinqToDB.Mapping;
using System;
using System.Linq;
using System.Collections.Generic;

namespace Server.Data
{
    [Table("UserGroups")]
    public class UserGroups
    {
        [Column]
        public Guid GroupId { get; set; }
        [Column]
        public Guid UserId { get; set; }

        public static List<UserGroups> GetUserGroups()
        {
            List<UserGroups> userGroups = null;
            try
            {
                using (var db = new DataConnection())
                {
                    var query = db.GetTable<UserGroups>();
                    userGroups = query.ToList();
                }
            }
            catch (Exception ex)
            {
                ServerLogger.Error(string.Format("UserGroups -> GetUserGroups: {0}", ex.Message));
            }
            return userGroups;
        }

        public static List<Guid> GetGroupsByUser(Guid userId)
        {
            List<Guid> userGroups = null;
            try
            {
                using (var db = new DataConnection())
                {
                    var query = db.GetTable<UserGroups>().Where(x => x.UserId == userId).Select(x => x.GroupId);
                    userGroups = query.ToList();
                }
            }
            catch (Exception ex)
            {
                ServerLogger.Error(string.Format("UserGroups -> GetGroupsByUser: {0}", ex.Message));
            }
            return userGroups;
        }

        public static List<Guid> GetGroups()
        {
            List<Guid> userGroups = null;
            try
            {
                using (var db = new DataConnection())
                {
                    var query = db.GetTable<UserGroups>().Select(x => x.GroupId);
                    userGroups = query.ToList();
                }
            }
            catch (Exception ex)
            {
                ServerLogger.Error(string.Format("UserGroups -> GetGroups: {0}", ex.Message));
            }
            return userGroups;
        }

        public static UserGroups GetUserGroup(UserGroups userGroup)
        {
            UserGroups userGroups = null;
            try
            {
                using (var db = new DataConnection())
                {
                    userGroups = db.GetTable<UserGroups>().Where(x => x.Equals(userGroup)).FirstOrDefault();
                }
            }
            catch (Exception ex)
            {
                ServerLogger.Error(string.Format("UserGroups -> GetUserGroups: {0}", ex.Message));
            }
            return userGroups;
        }

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
                ServerLogger.Error(string.Format("UserGroups -> Insert: {0}", ex.Message));
            }
            return result;
        }

        public static bool Delete(UserGroups userGroups)
        {
            bool result = true;
            try
            {
                using (var db = new DataConnection())
                {
                    db.Delete(userGroups);
                }
            }
            catch (Exception ex)
            {
                result = false;
                ServerLogger.Error(string.Format("UserGroups -> Delete: {0}", ex.Message));
            }
            return result;
        }
    }
}
