using Nancy;
using Nancy.Security;
using Server.Data;
using Server.Models;
using Server.Pipes;
using Server.Prize;
using System;
using System.Collections.Generic;
using System.Linq;
namespace Server.Modules
{
    public class MainModule : BaseModule
    {
        private ServerPrizeCache serverPrizeCache;
        
        public MainModule()
        {
            this.RequiresAuthentication();

            serverPrizeCache = new ServerPrizeCache();

            Get["/terminals"] = Terminals;
            Get["/terminalslogs"] = TerminalsLogs;
            Get["/prizefund"] = PrizeFund;
            Get["/terminals/fillbalance/{terminalId}"] = ViewFillBalance;
            Post["/terminals/fillbalance"] = FillBalance;
            Get["/terminals/changestatus/{terminalId}"] = ViewChangeStatus;
            Post["/terminals/changestatus"] = ChangeStatus;
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
                        Dictionary<Group, GroupInfo> dictGroupInfo = new Dictionary<Group, GroupInfo>();
                        foreach (var group in groups)
                        {
                            var terminalsOfGroup = terminals.Where(x => x.GroupId == group.Id);
                            GroupInfo groupInfo = new GroupInfo();
                            foreach (var terminal in terminalsOfGroup)
                            {
                                var counters = Counters.GetCountersByTerminal(terminal.Id);
                                TerminalCounters terminalCounters = new TerminalCounters();
                                terminalCounters.TerminalName = terminal.TerminalName;
                                terminalCounters.SumPrizeCounters = PrizeCounters.GetSumPrizeCountersByTerminal(terminal.Id);
                                terminalCounters.SumDiscountCardCounters = DiscountCardCounters.GetSumDiscountCardCountersByTerminal(terminal.Id);
                                if (counters != null)
                                {
                                    terminalCounters.Counters = counters;
                                }
                                else
                                {
                                    terminalCounters.Counters = new Counters() { TerminalId = terminal.Id, State = 255 };
                                }
                                groupInfo.TerminalCounters.Add(terminalCounters);
                                var logs = TerminalLog.GetLogsByTerminal(terminal.Id);
                                if (logs != null)
                                {
                                    foreach (var log in logs)
                                    {
                                        TerminalLogs terminalLogs = new TerminalLogs();
                                        terminalLogs.TerminalName = terminal.TerminalName;
                                        terminalLogs.TerminalLog = log;
                                        groupInfo.TerminalLogs.Add(terminalLogs);
                                    }
                                }
                            }
                            dictGroupInfo.Add(group, groupInfo);
                        }
                        Model.MainPage = new MainPageModel();
                        Model.MainPage.DictionaryGroupInfo = dictGroupInfo;
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

        private dynamic TerminalsLogs(dynamic parameters)
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
                        var idTerminals = terminals.Select(x => x.Id);
                        var logs = TerminalLog.GetLogs(idTerminals);
                        List<TerminalLogs> TerminalLogs = new List<TerminalLogs>();
                        if (logs != null)
                        {
                            foreach (var log in logs)
                            {
                                var terminal = terminals.FirstOrDefault(x => x.Id == log.TerminalId);
                                if (terminal != null)
                                {
                                    var group = groups.FirstOrDefault(x => x.Id == terminal.GroupId);
                                    if (group != null)
                                    {
                                        TerminalLogs terminalLogs = new TerminalLogs();
                                        terminalLogs.TerminalName = terminal.TerminalName;
                                        terminalLogs.GroupName = group.GroupName;
                                        terminalLogs.TerminalLog = log;

                                        TerminalLogs.Add(terminalLogs);
                                    }
                                }
                            }
                        }
                        Model.MainPage = new MainPageModel();
                        Model.MainPage.TerminalLogs = TerminalLogs;
                        return View["TerminalsLogs", Model];
                    }
                }
                else
                {
                    return Response.AsRedirect("~/error/" + (byte)MainErrors.NotGroups);
                }
            }
            return Response.AsRedirect("~/error/" + (byte)MainErrors.ErrorTerminalsLogs);
        }

        private dynamic PrizeFund(dynamic parameters)
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
                        var groupsPrizes = serverPrizeCache.Load();
                        if (groupsPrizes.Any())
                        {
                            List<ViewGroupPrize> listViewGroupPrize = new List<ViewGroupPrize>();
                            foreach (var group in groups)
                            {
                                var id = group.Id;
                                if (groupsPrizes.ContainsKey(id))
                                {
                                    ViewGroupPrize viewGroupPrize = new ViewGroupPrize();
                                    viewGroupPrize.GroupId = id;
                                    viewGroupPrize.GroupName = group.GroupName;
                                    var groupPrize = groupsPrizes[id];
                                    if (groupPrize != null)
                                    {
                                        viewGroupPrize.Status = true;
                                        viewGroupPrize.ValuePrize = groupPrize.ValuePrize;
                                        viewGroupPrize.Fund = groupPrize.Fund;
                                    }
                                    listViewGroupPrize.Add(viewGroupPrize);
                                }
                            }
                            Model.PrizeFundPage = new PrizeFundPageModel();
                            Model.PrizeFundPage.Groups = listViewGroupPrize;
                            return View["PrizeFund", Model];
                        }
                        else
                        {
                            return Response.AsRedirect("~/error/" + (byte)MainErrors.ErrorPrizeGroup);
                        }
                    }
                }
                else
                {
                    return Response.AsRedirect("~/error/" + (byte)MainErrors.NotGroups);
                }
            }
            return Response.AsRedirect("~/error/" + (byte)MainErrors.ErrorPrizeFund);
        }

        private dynamic ViewFillBalance(dynamic parameters)
        {
            var terminalIdString = (string)parameters.terminalId;
            if (!string.IsNullOrEmpty(terminalIdString))
            { 
                Guid terminalId;
                bool success = Guid.TryParse(terminalIdString, out terminalId);
                if (success)
                {
                    var terminal = Terminal.GetTerminalById(terminalId);
                    if (terminal != null)
                    {
                        var group = Group.GetGroupById(terminal.GroupId);
                        if (group != null)
                        {
                            Model.FillBalancePage = new FillBalancePageModel();
                            Model.FillBalancePage.Group = group;
                            Model.FillBalancePage.Terminal = terminal;

                            var settingsTerminal = SettingsTerminal.GetSettingsTerminalById(terminal.Id);
                            if (settingsTerminal != null)
                            {
                                Model.FillBalancePage.ImpulseBillAcceptor = settingsTerminal.ImpulseBillAcceptor;
                            }
                            return View["FillBalance", Model];
                        }
                    }
                }
            }
            return Response.AsRedirect("~/error/" + (byte)MainErrors.ErrorFillBalance);
        }

        private dynamic FillBalance(dynamic parameters)
        {
            var terminalIdString = (string)this.Request.Form.TerminalId;
            var cache = (UInt16)this.Request.Form.Cache;
            if (!string.IsNullOrEmpty(terminalIdString))
            { 
                Guid terminalId;
                bool success = Guid.TryParse(terminalIdString, out terminalId);
                if (success)
                {
                    PipeClient pipeClient = new PipeClient();
                    pipeClient.Write(new ServerPacket(terminalId, ServerPacketType.fillcache, cache));
                }
            }
            return Response.AsRedirect("~/terminals");
        }

        private dynamic ViewChangeStatus(dynamic parameters)
        {
            var terminalIdString = (string)parameters.terminalId;
            if (!string.IsNullOrEmpty(terminalIdString))
            {
                Guid terminalId;
                bool success = Guid.TryParse(terminalIdString, out terminalId);
                if (success)
                {
                    var terminal = Terminal.GetTerminalById(terminalId);
                    if (terminal != null)
                    {
                        var group = Group.GetGroupById(terminal.GroupId);
                        if (group != null)
                        {
                            Model.ChangeStatusPage = new ChangeStatusPageModel();
                            Model.ChangeStatusPage.Group = group;
                            Model.ChangeStatusPage.Terminal = terminal;

                            var settingsTerminal = SettingsTerminal.GetSettingsTerminalById(terminal.Id);
                            if (settingsTerminal != null)
                            {
                                Model.ChangeStatusPage.State = settingsTerminal.State;
                            }
                            return View["ChangeStatus", Model];
                        }
                    }
                }
            }
            return Response.AsRedirect("~/error/" + (byte)MainErrors.ErrorChangeStatus);
        }

        private dynamic ChangeStatus(dynamic parameters)
        {
            var terminalIdString = (string)this.Request.Form.TerminalId;
            var state = (byte)this.Request.Form.State;
            if (!string.IsNullOrEmpty(terminalIdString))
            {
                Guid terminalId;
                bool success = Guid.TryParse(terminalIdString, out terminalId);
                if (success)
                {
                    var settingsTerminal = SettingsTerminal.GetSettingsTerminalById(terminalId);
                    if (settingsTerminal != null)
                    {
                        settingsTerminal.State = state;
                        SettingsTerminal.Update(settingsTerminal);
                        PipeClient pipeClient = new PipeClient();
                        pipeClient.Write(new ServerPacket(terminalId, ServerPacketType.changeStatus, state));
                    }
                }
            }
            return Response.AsRedirect("~/terminals");
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
