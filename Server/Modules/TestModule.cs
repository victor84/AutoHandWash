using Nancy;

namespace Server.Modules
{
    public class TestModule : NancyModule
    {
        public TestModule()
        {
            Get["/"] = TestSignalR;
        }

        private dynamic TestSignalR(dynamic parameters)
        {
            return View["TestSignalR"];
        }
    }
}
