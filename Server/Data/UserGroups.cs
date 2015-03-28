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
                Console.WriteLine("UserGroups -> GetUserGroups: {0}", ex);
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
                Console.WriteLine("UserGroups -> GetUserGroups: {0}", ex);
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
                Console.WriteLine("UserGroups -> Insert: {0}", ex);
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
                Console.WriteLine("UserGroups -> Delete: {0}", ex);
            }
            return result;
        }
    }
}
