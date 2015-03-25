using Nancy.Security;
using Server.Data;

namespace Server.Modules
{
    public class AdminModule : BaseModule
    {
        public AdminModule()
            : base("admin")
        {
            this.RequiresAuthentication();
            this.RequiresClaims(new string[] { User.adminClaim });
            Get["/"] = Index;
            Get["/userGroups"] = UserGroups;
        }

        private dynamic Index(dynamic parameters)
        {
            return View["AdminIndex", Model];
        }

        private dynamic UserGroups(dynamic parameters)
        {
            return View["UserGroups", Model];
        }
    }
}
