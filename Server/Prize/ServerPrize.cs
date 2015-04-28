﻿using Server.Data;
using Server.Pipes;
using System;
using System.Collections.Generic;

namespace Server.Prize
{
    public class ServerPrize
    {
        private object _lock;
        private Dictionary<Guid, GroupPrize> groups;
        public ServerPrize()
        {
            _lock = new object();
            groups = new Dictionary<Guid, GroupPrize>();
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
                    var settingsGroups = SettingsGroup.GetSettingsGroupById(groupId);
                    if (settingsGroups != null)
                    {
                        if (settingsGroups.HasPresent)
                        {
                            groupPrize = CreateGroupPrize(settingsGroups);
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
                        var value = input * groupPrize.Percent;
                        groupPrize.AddInFund(value);
                    }
                }
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
                }
            }
        }

        private GroupPrize CreateGroupPrize(SettingsGroup settingsGroup)
        {
            GroupPrize groupPrize = new GroupPrize();
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

        public void ChangeSettingsGroup(SettingsGroup settingsGroup)
        {
            lock (_lock)
            {
                var groupId = settingsGroup.GroupId;
                if (groups.ContainsKey(groupId))
                {
                    if (settingsGroup.HasPresent)
                    {
                        var groupPrize = groups[groupId];
                        groupPrize = CreateGroupPrize(settingsGroup);
                    }
                }
            }
        }
    }
}
