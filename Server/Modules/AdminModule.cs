using Nancy.Security;

namespace Server.Modules
{
    public class AdminModule : BaseModule
    {
        public AdminModule()
        {
            this.RequiresAuthentication();
            Get["/admin"] = Admin;
        }

        private dynamic Admin(dynamic parameters)
        {
            return View["Admin", Model];
        }
    }
}
