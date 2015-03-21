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
                Console.WriteLine("GetTerminalsByName: {0}", ex);
            }
            return terminals;
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
                Console.WriteLine("Terminal -> Insert: {0}", ex);
            }
            return result;
        }
    }
}
