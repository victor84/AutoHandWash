using Server.Data;
using Server.Pipes;
using System;
using System.Linq;
using System.Collections.Generic;

namespace Server.Prize
{
    public class ServerPrize
    {
        private object _lock;
        private ServerPrizeCache cache;
        private Dictionary<Guid, GroupPrize> groups;
        public ServerPrize()
        {
            _lock = new object();
            cache = new ServerPrizeCache();
            groups = new Dictionary<Guid, GroupPrize>();
            InitGroups();
            SaveGroups();
        }

        public void CommonInputExtended(Terminal terminal, uint input)
        { 
            lock(_lock)
            {
                var terminalId = terminal.Id;
                var groupId = terminal.GroupId;
                GroupPrize groupPrize = null;
                if (!groups.ContainsKey(groupId))
                {
                    var group = Group.GetGroupById(groupId);
                    var settingsGroups = SettingsGroup.GetSettingsGroupById(groupId);
                    if (group != null && settingsGroups != null)
                    {
                        if (settingsGroups.HasPresent)
                        {
                            groupPrize = CreateGroupPrize(group, settingsGroups);
                        }
                        groups.Add(groupId, groupPrize);
                    }
                }
                else
                {
                    groupPrize = groups[groupId];
                }
                if (groupPrize != null)
                {
                    if (!groupPrize.WaitConfirm)
                    {
                        groupPrize.LastTerminalId = terminalId;
                        var value = input * groupPrize.Percent / 100;
                        groupPrize.AddInFund(value);
                    }
                }
                SaveGroups();
            }
        }

        public void PrizeConfirmed(Guid groupId)
        {
            lock (_lock)
            {
                if (groups.ContainsKey(groupId))
                {
                    var groupPrize = groups[groupId];
                    groupPrize = null;
                    groups.Remove(groupId);
                    SaveGroups();
                }
            }
        }

        public void PrizeNotConfirmed(Terminal terminal)
        {
            lock (_lock)
            {
                var groupId = terminal.GroupId;
                if (groups.ContainsKey(groupId))
                {
                    var groupPrize = groups[groupId];
                    if (groupPrize != null)
                    {
                        groupPrize.WaitConfirm = false;
                    }
                    SaveGroups();
                }
            }
        }

        public void ChangeSettingsGroup(SettingsGroup settingsGroup)
        {
            lock (_lock)
            {
                var groupId = settingsGroup.GroupId;
                if (groups.ContainsKey(groupId))
                {
                    if (settingsGroup.HasPresent)
                    {
                        var group = Group.GetGroupById(groupId);
                        if (group != null)
                        {
                            groups[groupId] = CreateGroupPrize(group, settingsGroup);
                        }
                    }
                    else
                    {
                        groups[groupId] = null;
                    }
                    SaveGroups();
                }
            }
        }

        private void InitGroups()
        {
            var settingsGroups = SettingsGroup.GetSettingsGroups();
            if (settingsGroups != null && settingsGroups.Any())
            {
                foreach (var s in settingsGroups)
                {
                    GroupPrize groupPrize = null;
                    if (s.HasPresent)
                    {
                        var group = Group.GetGroupById(s.GroupId);
                        if (group != null)
                        {
                            groupPrize = CreateGroupPrize(group, s);
                        }
                    }
                    groups.Add(s.GroupId, groupPrize);
                }
            }
        }

        private void SaveGroups()
        {
            cache.Save(groups);
        }

        private GroupPrize CreateGroupPrize(Group group, SettingsGroup settingsGroup)
        {
            GroupPrize groupPrize = new GroupPrize();
            groupPrize.GroupId = group.Id;
            groupPrize.GroupName = group.GroupName;
            groupPrize.Percent = settingsGroup.PercentForPresent;
            Random rnd = new Random();
            int minValue = (int)(settingsGroup.LowerBoundForRandomSum / 10);
            int maxValue = (int)(settingsGroup.UpperBoundForRandomSum / 10);
            groupPrize.ValuePrize = (ushort)(10 * rnd.Next(minValue, maxValue));
            groupPrize.ValuePrizeReached += new EventHandler(OnValuePrizeReached);
            return groupPrize;
        }

        private void OnValuePrizeReached(object sender, EventArgs e)
        {
            var groupPrize = (GroupPrize)sender;
            PipeClient pipeClient = new PipeClient();
            pipeClient.Write(new ServerPacket(groupPrize.LastTerminalId, ServerPacketType.prize, groupPrize.ValuePrize));
        }
    }
}
