﻿using LinqToDB;
using LinqToDB.Data;
using LinqToDB.Mapping;
using System;
using System.Collections.Generic;
using System.Linq;

namespace Server.Data
{
    [Table("Terminals")]
    public class Terminal
    {
        [Column]
        public Guid Id { get; set; }
        [Column]
        public string TerminalName { get; set; }
        [Column]
        public Guid GroupId { get; set; }

        public static List<Terminal> GetTerminals()
        {
            List<Terminal> terminals = null;
            try
            {
                using (var db = new DataConnection())
                {
                    var query = db.GetTable<Terminal>();
                    terminals = query.ToList();
                }
            }
            catch (Exception ex)
            {
                ServerLogger.Error(string.Format("Terminal -> GetTerminals: {0}", ex.Message));
            }
            return terminals;
        }

        public static List<Terminal> GetTerminalsByGroup(IEnumerable<Guid> ids)
        {
            List<Terminal> terminals = null;
            try
            {
                using (var db = new DataConnection())
                {
                    var query = db.GetTable<Terminal>().Where(x => ids.Contains(x.GroupId));
                    terminals = query.ToList();
                }
            }
            catch (Exception ex)
            {
                ServerLogger.Error(string.Format("Terminal -> GetTerminalsByGroup: {0}", ex.Message));
            }
            return terminals;
        }

        public static IEnumerable<Terminal> GetTerminalsByName(string terminalName)
        {
            IEnumerable<Terminal> terminals = null;
            try
            {
                using (var db = new DataConnection())
                {
                    terminals = db.GetTable<Terminal>().Where(x => x.TerminalName == terminalName);
                }
            }
            catch (Exception ex)
            {
                ServerLogger.Error(string.Format("Terminal -> GetTerminalsByName: {0}", ex.Message));
            }
            return terminals;
        }

        public static Terminal GetTerminalById(Guid id)
        {
            Terminal terminal = null;
            try
            {
                using (var db = new DataConnection())
                {
                    terminal = db.GetTable<Terminal>().Where(x => x.Id == id).FirstOrDefault();
                }
            }
            catch (Exception ex)
            {
                ServerLogger.Error(string.Format("Terminal -> GetTerminalById: {0}", ex.Message));
            }
            return terminal;
        }

        public static bool Insert(Terminal terminal)
        {
            bool result = true;
            try
            {
                using (var db = new DataConnection())
                {
                    db.Insert(terminal);
                }
            }
            catch (Exception ex)
            {
                result = false;
                ServerLogger.Error(string.Format("Terminal -> Insert: {0}", ex.Message));
            }
            return result;
        }
    }
}
