using MySql.Data.MySqlClient;
using System;
namespace Server.Services
{
    public class Auth : IAuth
    {
        private string connectionString;

        public Auth()
        {
            connectionString = @"server=localhost;userid=root;password=xTlaQ77w;database=dbautohandwash";
        }

        public Auth(string server, string user, string password, string dbName)
        {
            connectionString = string.Format("server={0};userid={1};password={2};database={3}", server, user, password, dbName);
        }

        public User GetUserById(Guid id)
        {
            User user = null;
            MySqlConnection conn = null;
            MySqlDataReader rdr = null;
            try
            {
                conn = new MySqlConnection(connectionString);
                conn.Open();
                string stm = @"SELECT * FROM users WHERE Id = '" + id + "'";
                MySqlCommand cmd = new MySqlCommand(stm, conn);
                rdr = cmd.ExecuteReader();
                while (rdr.Read())
                {
                    string userName = (string)rdr["UserName"];
                    string password = (string)rdr["Password"];
                    string claim = (string)rdr["Claim"];
                    user = new User()
                    {
                        Id = id,
                        UserName = userName,
                        Password = password,
                        Claims = new string[] { claim }
                    };
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error: {0}", ex.ToString());
            }
            finally
            {
                if (rdr != null)
                {
                    rdr.Close();
                }
                if (conn != null)
                {
                    conn.Close();
                }
            }
            return user;
        }

        public User GetUserByName(string userName)
        {
            User user = null;
            MySqlConnection conn = null;
            MySqlDataReader rdr = null;
            try
            {
                conn = new MySqlConnection(connectionString);
                conn.Open();
                string stm = @"SELECT * FROM users WHERE UserName = '" + userName + "'";
                MySqlCommand cmd = new MySqlCommand(stm, conn);
                rdr = cmd.ExecuteReader();
                while (rdr.Read())
                {
                    Guid id = (Guid)rdr["Id"];
                    string password = (string)rdr["Password"];
                    string claim = (string)rdr["Claim"];
                    user = new User()
                    {
                        Id = id,
                        UserName = userName,
                        Password = password,
                        Claims = new string[] { claim }
                    };
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error: {0}", ex.ToString());
            }
            finally
            {
                if (rdr != null)
                {
                    rdr.Close();
                }
                if (conn != null)
                {
                    conn.Close();
                }
            }
            return user;
        }
    }
}
