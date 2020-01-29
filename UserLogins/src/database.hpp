#pragma once

#if !defined(Database_header)
#define Database_header

#include <cstdio>               // printf, putchar
#include <cstdlib>              // exit

#include <string>
#include <vector>
#include <utility>              // forward
#include <fstream>
#include <iomanip>              // setw

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

    void import(const std::string&);

    void save(const std::string&) const;

    int execute_sql(const std::string&);

    void update_db();

    void insert_into_db(const Account&);

    template<typename... Args>
    void insert_into_db(Args&&...);

    void query_db(const std::string&);

    void display_db();

    void delete_db();

private:
    bool is_duplicate(const Account&);
    void open_db_connection();
    void close_db_connection();
    void create_db();
    void create_table_into_db();

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