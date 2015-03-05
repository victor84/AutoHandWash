using Nancy;
using Nancy.Authentication.Forms;
using Nancy.Security;
using System;

namespace Server
{
    public class UserMapper : IUserMapper
    {
        public IUserIdentity GetUserFromIdentifier(Guid identifier, NancyContext context)
        {
            // TODO: Здесь надо достать пользователя из БД по параметру identifier
            // var user = _dbService.GetById(identifier);
            // а пока только тестовый объект
            UserIdentity user = new UserIdentity() 
            {
                Claims = new string[] { "admin" },
                UserName = "Ivan",
            };
            if (user == null)
                return null;

            return new UserIdentity { UserName = user.UserName, Claims = user.Claims };
        }
    }
}
