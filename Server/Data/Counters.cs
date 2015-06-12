using LinqToDB;
using LinqToDB.Data;
using LinqToDB.Mapping;
using System;
using System.Linq;
using System.Collections.Generic;

namespace Server.Data
{
    [Table("Counters")]
    public class Counters
    {
        private byte state;
        
        [Column]
        public Guid Id { get; set; }
        [Column]
        public Guid TerminalId { get; set; }
        [Column]
        public DateTime DateTimeServerEvent { get; set; }
        [Column]
        public DateTime DateTimeTerminalEvent { get; set; }
        [Column]
        public byte State
        {
            get
            {
                return state;
            }
            set
            {
                state = value;
            }
        }
        [Column]
        public uint CommonInput { get; set; }
        [Column]
        public uint Balance { get; set; }
        [Column]
        public uint PressurizedWater { get; set; }
        [Column]
        public uint NoPressurizedWater { get; set; }
        [Column]
        public uint Foam { get; set; }
        [Column]
        public uint Wax { get; set; }
        [Column]
        public uint Osmose { get; set; }
        [Column]
        public uint Vacuum { get; set; }
        [Column]
        public uint Air { get; set; }

        #region Для отображения в интерфейсе сервера

        public string ViewState
        {
            get
            {
                string result = string.Empty;
                switch (state)
                {
                    case 0:
                        result = "Работает";
                        break;
                    case 1:
                        result = "Простаивает";
                        break;
                    case 2:
                        result = "Сломан";
                        break;
                    case 3:
                        result = "Заблокирован";
                        break;
                    default:
                        result = "Неизвестно";
                        break;
                }
                return result;
            }
        }

        public string ViewPressurizedWater 
        { 
            get
            {
                TimeSpan timeSpan = TimeSpan.FromSeconds(PressurizedWater);
                string result = timeSpan.ToString();
                return result;
            } 
        }

        public string ViewNoPressurizedWater
        {
            get
            {
                TimeSpan timeSpan = TimeSpan.FromSeconds(NoPressurizedWater);
                string result = timeSpan.ToString();
                return result;
            }
        }

        public string ViewFoam
        {
            get
            {
                TimeSpan timeSpan = TimeSpan.FromSeconds(Foam);
                string result = timeSpan.ToString();
                return result;
            }
        }

        public string ViewWax
        {
            get
            {
                TimeSpan timeSpan = TimeSpan.FromSeconds(Wax);
                string result = timeSpan.ToString();
                return result;
            }
        }

        public string ViewOsmose
        {
            get
            {
                TimeSpan timeSpan = TimeSpan.FromSeconds(Osmose);
                string result = timeSpan.ToString();
                return result;
            }
        }

        public string ViewVacuum
        {
            get
            {
                TimeSpan timeSpan = TimeSpan.FromSeconds(Vacuum);
                string result = timeSpan.ToString();
                return result;
            }
        }

        public string ViewAir
        {
            get
            {
                TimeSpan timeSpan = TimeSpan.FromSeconds(Air);
                string result = timeSpan.ToString();
                return result;
            }
        }

        #endregion

        public static bool Insert(Counters counters)
        {
            bool result = true;
            try
            {
                using (var db = new DataConnection())
                {
                    db.Insert(counters);
                }
            }
            catch (Exception ex)
            {
                result = false;
                ServerLogger.Error(string.Format("Counters -> Insert: {0}", ex.Message));
            }
            return result;
        }

        public static Counters GetCountersByTerminal(Guid id)
        {
            Counters counters = null;
            try
            {
                using (var db = new DataConnection())
                {
                    var table = db.GetTable<Counters>();
                    var query = table.Where(x => x.TerminalId == id);
                    var queryMaxDateTime = query.Where(x => x.DateTimeServerEvent == query.Max(s => s.DateTimeServerEvent));
                    var queryCommonInput = queryMaxDateTime.Where(x => x.CommonInput == queryMaxDateTime.Max(s => s.CommonInput));
                    counters = queryCommonInput.FirstOrDefault();
                }
            }
            catch (Exception ex)
            {
                ServerLogger.Error(string.Format("Counters -> GetCounters: {0}", ex.Message));
            }
            return counters;
        }
    }
}
