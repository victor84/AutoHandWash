using Nancy;
using Server.Data;
using Server.Models;
using System;
using System.Dynamic;
using System.Linq;

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
                Model.MasterPage.Year = DateTime.Now.Year;
                bool isAuthenticated = (ctx.CurrentUser != null);
                Model.MasterPage.IsAuthenticated = isAuthenticated;
                if (isAuthenticated)
                {
                    Model.MasterPage.UserName = ctx.CurrentUser.UserName;
                    var claim = ctx.CurrentUser.Claims.FirstOrDefault();
                    if (claim != null)
                    {
                        Model.MasterPage.Claim = claim;
                        Model.MasterPage.IsAdmin = (claim == User.adminClaim) ? true : false;
                    }
                }
                return null;
            });
        }
    }
}
