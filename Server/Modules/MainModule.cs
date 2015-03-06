using Nancy.Security;
namespace Server.Modules
{
    public class MainModule : BaseModule
    {
        public MainModule()
        {
            this.RequiresAuthentication();
            Get["/groups"] = TestSignalR;
            Get["/washings"] = Washings;
        }

        private dynamic TestSignalR(dynamic parameters)
        {
            return View["TestSignalR", Model];
        }

        private dynamic Washings(dynamic parameters)
        {
            return View["Washings", Model];
        }
    }
}
