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
            try
            {
                using (var conn = new MySqlConnection(connectionString))
                {
                    using (var cmd = conn.CreateCommand())
                    {
                        conn.Open();
                        cmd.CommandText = @"SELECT * FROM users WHERE Id = '" + id + "'";
                        using (var reader = cmd.ExecuteReader())
                        {
                            while (reader.Read())
                            {
                                string userName = (string)reader["UserName"];
                                string password = (string)reader["Password"];
                                string claim = (string)reader["Claim"];
                                user = new User()
                                {
                                    Id = id,
                                    UserName = userName,
                                    Password = password,
                                    Claims = new string[] { claim }
                                };
                            }
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("GetUserById: {0}", ex.ToString());
            }
            return user;
        }

        public User GetUserByName(string userName)
        {
            User user = null;
            try
            {
                using (var conn = new MySqlConnection(connectionString))
                {
                    using (var cmd = conn.CreateCommand())
                    {
                        conn.Open();
                        cmd.CommandText = @"SELECT * FROM users WHERE UserName = '" + userName + "'";
                        using (var reader = cmd.ExecuteReader())
                        {
                            while (reader.Read())
                            {
                                Guid id = (Guid)reader["Id"];
                                string password = (string)reader["Password"];
                                string claim = (string)reader["Claim"];
                                user = new User()
                                {
                                    Id = id,
                                    UserName = userName,
                                    Password = password,
                                    Claims = new string[] { claim }
                                };
                            }
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("GetUserByName: {0}", ex.ToString());
            }
            return user;
        }
    }
}
