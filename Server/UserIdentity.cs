using Nancy.Security;
using System.Collections.Generic;

namespace Server
{
    public class UserIdentity : IUserIdentity
    {
        public IEnumerable<string> Claims { get; set; }
        public string UserName { get; set; }
    }
}
