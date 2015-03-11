using Nancy.Security;
namespace Server.Modules
{
    public class MainModule : BaseModule
    {
        public MainModule()
        {
            this.RequiresAuthentication();
            Get["/groups"] = TestSignalR;
            Get["/terminals"] = Terminals;
        }

        private dynamic TestSignalR(dynamic parameters)
        {
            return View["TestSignalR", Model];
        }

        private dynamic Terminals(dynamic parameters)
        {
            return View["Terminals", Model];
        }
    }
}
