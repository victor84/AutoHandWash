using Nancy;
using Nancy.Authentication.Forms;
using Nancy.Security;
using Server.Services;
using System;

namespace Server
{
    public class UserMapper : IUserMapper
    {
        private readonly IAuth _auth;

        public UserMapper(IAuth auth)
        {
            _auth = auth;
        }
        
        public IUserIdentity GetUserFromIdentifier(Guid identifier, NancyContext context)
        {
            var user = _auth.GetUserById(identifier);
            return user;
        }

    }
}
