using LinqToDB.Data;
using LinqToDB.Mapping;
using Nancy.Security;
using System;
using System.Linq;
using System.Collections.Generic;

namespace Server
{
    [Table("Users")]
    public class User : IUserIdentity
    {
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
                Console.WriteLine("GetUserById: {0}", ex);
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
                Console.WriteLine("GetUserByName: {0}", ex);
            }
            return user;
        }
    }
}
