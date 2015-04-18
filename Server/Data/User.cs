using LinqToDB;
using LinqToDB.Data;
using LinqToDB.Mapping;
using Nancy.Security;
using System;
using System.Linq;
using System.Collections.Generic;

namespace Server.Data
{
    [Table("Users")]
    public class User : IUserIdentity
    {
        public static string adminClaim = "admin";
        public static string userClaim = "user";
        
        [Column]
        public Guid Id { get; set; }
        [Column]
        public string UserName { get; set; }
        [Column]
        public string Password { get; set; }
        [Column]
        public string Claim { get; set; }

        public IEnumerable<string> Claims 
        {
            get 
            {
                IEnumerable<string> result = null;
                string claim = Claim;
                if (!string.IsNullOrEmpty(claim))
                {
                    result = new string[] { claim };
                }
                return result;
            } 
        }

        public static User GetUserById(Guid id)
        {
            User user = null;
            try
            {
                using (var db = new DataConnection())
                {
                    user = db.GetTable<User>().Where(x => x.Id == id).FirstOrDefault();
                }
            }
            catch (Exception ex)
            {
                ServerLogger.Error(string.Format("User -> GetUserById: {0}", ex.Message));
            }
            return user;
        }

        public static User GetUserByName(string userName)
        {
            User user = null;
            try
            {
                using (var db = new DataConnection())
                {
                    user = db.GetTable<User>().Where(x => x.UserName == userName).FirstOrDefault();
                }
            }
            catch (Exception ex)
            {
                ServerLogger.Error(string.Format("User -> GetUserByName: {0}", ex.Message));
            }
            return user;
        }

        public static List<User> GetUsers()
        {
            List<User> users = null;
            try
            {
                using (var db = new DataConnection())
                {
                    var query = db.GetTable<User>().Where(x => x.Claim != adminClaim);
                    users = query.ToList();
                }
            }
            catch (Exception ex)
            {
                ServerLogger.Error(string.Format("User -> GetUsers: {0}", ex.Message));
            }
            return users;
        }

        public static bool Insert(User user)
        {
            bool result = true;
            try
            {
                using (var db = new DataConnection())
                {
                    db.Insert(user);
                }
            }
            catch (Exception ex)
            {
                result = false;
                ServerLogger.Error(string.Format("User -> Insert: {0}", ex.Message));
            }
            return result;
        }

        public static bool Delete(User user)
        {
            bool result = true;
            try
            {
                using (var db = new DataConnection())
                {
                    db.Delete(user);
                }
            }
            catch (Exception ex)
            {
                result = false;
                ServerLogger.Error(string.Format("User -> Delete: {0}", ex.Message));
            }
            return result;
        }
    }
}
