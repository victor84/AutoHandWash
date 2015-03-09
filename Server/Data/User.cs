using Nancy.Security;
using System;
using System.Collections.Generic;

namespace Server
{
    public class User : IUserIdentity
    {
        public Guid Id { get; set; }
        public string UserName { get; set; }
        public string Password { get; set; }
        public IEnumerable<string> Claims { get; set; }
    }
}
