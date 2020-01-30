#pragma once

#if !defined(Database_header)
#define Database_header

#include <cstdio>               // printf, putchar
#include <cstdlib>              // exit

#include <iostream>
#include <string>
#include <vector>
#include <utility>              // move, forward
#include <fstream>
#include <iomanip>              // setw
#include <iterator>             // istreambuf_iterator

#if __GNUC__ >= 8
    #include <filesystem>
    namespace fs = std::filesystem;
#else
    #include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
#endif

#include <sqlite3.h>            // sqlite3_open, sqlite3_close, sqlite3_exec
#include <nlohmann/json.hpp>    // parse, items, object

#include "account.hpp"

using json = nlohmann::json;
using Table = std::vector<std::vector<std::string>>;

class Database {
public:
    Database();
    explicit Database(
        const std::string&,
        const std::string&,
        const std::string& = "config.json",
        const std::string& = "account.db");

    template<typename... Args>
    bool authenticate(Args&&...) const &;

    template<typename... Args>
    void add_user(Args&&...) &;

    template<typename... Args>
    void assign_admin(Args&&...) &;

    virtual ~Database() {}

    void import_json(const std::string&);

    void export_as_json(const std::string&) const;

    int open_db_connection();
    static int open_db_connection(const std::string&, sqlite3*);

    int close_db_connection();
    static int close_db_connection(sqlite3*);

    int execute_sql(
        const std::string&,
        int(*)(void*, int, char**, char**),
        Table* = nullptr);

    static int execute_sql(
        sqlite3*,
        const std::string&,
        int(*)(void*, int, char**, char**),
        Table* = nullptr);

    int create_table_into_db();
    static int create_table_into_db(sqlite3*, const std::string&);

    int update_db();
    static int update_db(sqlite3*);

    int insert_into_db(const Account&);
    int insert_into_db(Account&&);

    template<typename... Args>
    int insert_into_db(Args&&...);
    
    static int insert_into_db(sqlite3*, const Account&);
    static int insert_into_db(sqlite3*, Account&&);

    template<typename... Args>
    static int insert_into_db(sqlite3*, Args&&...);

    int display_db();
    static int display_db(sqlite3*);

    Table retrieve_db();
    static Table retrieve_db(sqlite3*);

    int delete_db();
    static int delete_db(sqlite3*);

public:
    bool is_duplicate(const Account&);
    void create_db();

    static bool startswith(const std::string&, const std::string&);
    static bool endswith(const std::string&, const std::string&);
    static std::string get_db_name(
        const std::string&, const std::string&, const std::string&);

    Account m_administrator;
    std::vector<Account> m_accounts;
    std::string m_dbname;
    sqlite3* m_db;
};

#include "database.tpp"     // implementation of template members

#endif