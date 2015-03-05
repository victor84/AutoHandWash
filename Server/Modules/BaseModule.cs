using Nancy;
using Server.Models;
using System.Dynamic;

namespace Server.Modules
{
    public abstract class BaseModule : NancyModule
    {
        public dynamic Model = new ExpandoObject();

        public BaseModule()
        {
            SetupModelDefaults();
        }

        public BaseModule(string modulepath)
            : base(modulepath)
        {
            SetupModelDefaults();
        }

        private void SetupModelDefaults()
        {
            Before.AddItemToEndOfPipeline(ctx =>
            {
                Model.MasterPage = new MasterPageModel();
                Model.MasterPage.Title = "Автомойки";
                Model.MasterPage.ProjectName = "Автомойки";
                Model.MasterPage.IsAuthenticated = (ctx.CurrentUser != null);
                return null;
            });
        }
    }
}
