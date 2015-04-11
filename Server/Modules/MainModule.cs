using Nancy;
using Nancy.Security;
using Server.Data;
using Server.Models;
using System.Collections.Generic;
using System.Linq;
namespace Server.Modules
{
    public class MainModule : BaseModule
    {
        public MainModule()
        {
            this.RequiresAuthentication();
            Get["/terminals"] = Terminals;
            Get["/error/{type}"] = ViewError;
        }

        private dynamic Terminals(dynamic parameters)
        {
            string userName = Model.MasterPage.UserName;
            var user = User.GetUserByName(userName);
            if (user != null)
            {
                List<Group> groups = null;
                if (user.Claim == User.adminClaim)
                {
                    groups = Group.GetGroups();
                }
                else
                {
                    var idGroups = UserGroups.GetGroupsByUser(user.Id);
                    if (idGroups != null)
                    {
                        groups = Group.GetGroups(idGroups);
                    }
                }
                if (groups != null && groups.Any())
                {
                    var idGroups = groups.Select(x => x.Id);
                    var terminals = Terminal.GetTerminalsByGroup(idGroups);
                    if (terminals != null && terminals.Any())
                    {
                        Dictionary<Group, List<TerminalCounters>> dictionaryTerminalCounters = new Dictionary<Group, List<TerminalCounters>>();
                        foreach (var group in groups)
                        {
                            var terminalsOfGroup = terminals.Where(x => x.GroupId == group.Id);
                            List<TerminalCounters> listTerminalCounters = new List<TerminalCounters>();
                            foreach (var terminal in terminalsOfGroup)
                            {
                                var counters = Counters.GetCountersByTerminal(terminal.Id);
                                if (counters != null)
                                {
                                    TerminalCounters terminalCounters = new TerminalCounters();
                                    terminalCounters.TerminalName = terminal.TerminalName;
                                    terminalCounters.Counters = counters;
                                    listTerminalCounters.Add(terminalCounters);
                                }
                            }
                            dictionaryTerminalCounters.Add(group, listTerminalCounters);
                        }
                        Model.MainPage = new MainPageModel();
                        Model.MainPage.DictionaryTerminalCounters = dictionaryTerminalCounters;
                        return View["Terminals", Model];
                    }
                }
                else
                {
                    return Response.AsRedirect("~/error/" + (byte)MainErrors.NotGroups);
                }
            }
            return Response.AsRedirect("~/error/" + (byte)MainErrors.ErrorTerminals);
        }

        private dynamic ViewError(dynamic parameters)
        {
            var type = (byte)parameters.type;
            Model.MainPage = new MainPageModel();
            Model.MainPage.MainError = new MainError();
            Model.MainPage.MainError.Type = (MainErrors)type;
            return View["MainError", Model];
        }
    }
}
