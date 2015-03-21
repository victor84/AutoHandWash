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
            var user = User.GetUserById(identifier);
            return user;
        }
    }
}
