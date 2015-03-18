using LinqToDB.Data;
using Nancy;
using Nancy.Authentication.Forms;
using Nancy.Security;
using System;
using System.Linq;

namespace Server
{
    public class UserMapper : IUserMapper
    {
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
        
        public IUserIdentity GetUserFromIdentifier(Guid identifier, NancyContext context)
        {
            var user = GetUserById(identifier);
            return user;
        }
    }
}
