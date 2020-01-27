#pragma once

#if !defined(Database_header)
#define Database_header

#include <cstring>

#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <iomanip>
#include <sqlite3.h>
#include <nlohmann/json.hpp>

#include "account.hpp"

using json = nlohmann::json;

class Database {
public:
    Database();
    explicit Database(
        const std::string& username,
        const std::string& password,
        const std::string& filename = "config.json");

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

private:
    bool is_duplicate(const Account&);

    Account m_administrator;
    std::vector<Account> m_accounts;
    std::size_t loaded;
};

#endif