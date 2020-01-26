#pragma once

#if !defined(Database_header)
#define Database_header

#include <string>
#include <vector>
#include <utility>
#include <fstream>
#include <nlohmann/json.hpp>

#include "account.hpp"

using json = nlohmann::json;

class Database {
public:
    Database();

    explicit Database(
        const std::string& username,
        const std::string& password);
        
    explicit Database(
        std::string&& username,
        std::string&& password);


    bool authenticate(
        const std::string&, const std::string&) const &;

    bool authenticate(
        std::string&&, std::string&&) const &&;

    bool authenticate(
        const std::string&, const std::string&, const std::string&) const &;

    bool authenticate(
        std::string&&, std::string&&, std::string&&) const &&;


    void add_user(const std::string&, const std::string&) &;
    void add_user(std::string&&, std::string&&) &&;

    void assign_admin(const std::string&, const std::string&) &;
    void assign_admin(std::string&&, std::string&&) &&;

    void import();
    void import(const std::string&);

    void save() const;
    void save_as(const std::string&) const;

private:
    Account m_administrator;
    std::vector<Account> m_accounts;
};

#endif