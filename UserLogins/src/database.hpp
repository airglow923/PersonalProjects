#pragma once

#if !defined(Database_header)
#define Database_header

#include <cstdio>               // printf, putchar
#include <cstdlib>              // exit

#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <iomanip>
#include <sqlite3.h>            // sqlite3_open, sqlite3_close, sqlite3_exec
#include <nlohmann/json.hpp>    // parse, items, object

#include "account.hpp"

using json = nlohmann::json;

class Database {
public:
    Database();
    explicit Database(
        const std::string&,
        const std::string&,
        const std::string& = "config.json",
        const std::string& = "account.db");

    bool authenticate(
        const std::string&,
        const std::string&,
        const std::string& = "SHA256") const &;

    void add_user(
        const std::string&,
        const std::string&,
        const std::string& = "SHA256") &;

    void assign_admin(
        const std::string&,
        const std::string&,
        const std::string& = "SHA256") &;

    void import(const std::string&);
    void save(const std::string&) const;
    int execute_sql(const std::string&);
    void update_db();
    void add_user_to_db(const Account&);
    void add_user_to_db(
        const std::string&, const std::string&, const std::string&);
    void query_db(const std::string&);

private:
    bool is_duplicate(const Account&);
    void open_db_connection();
    void close_db_connection();
    void create_db();
    void create_table_inside_db();

    static bool startswith(const std::string&, const std::string&);
    static bool endswith(const std::string&, const std::string&);
    static std::string get_db_name(
        const std::string&, const std::string&, const std::string&);

    Account m_administrator;
    std::vector<Account> m_accounts;
    std::string m_dbname;
    sqlite3* m_db;
};

#endif