using Nancy.Security;

namespace Server.Modules
{
    public class UserModule : BaseModule
    {
        public UserModule()
        {
            this.RequiresAuthentication();
            Get["/users"] = Users;
        }

        private dynamic Users(dynamic parameters)
        {
            return View["Users", Model];
        }
    }
}
