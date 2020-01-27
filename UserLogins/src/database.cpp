#include "database.hpp"

Database::Database()
    : m_administrator(Account("admin", "admin")), loaded(0)
{
    import("config.json");
}

Database::Database(
    const std::string& username,
    const std::string& password,
    const std::string& filename)
    : m_administrator(Account(username, password)), loaded(0)
{
    import(filename);
}

bool Database::authenticate(
    const std::string& username, const std::string& password) const &
{
    Account access(username, password);

    for (const auto& account : m_accounts)
        if (access == account)
            return true;
    return false;
}

bool Database::authenticate(
    const std::string& username,
    const std::string& password,
    const std::string& algorithm) const &
{
    Account access(username, password, algorithm);

    for (const auto& account : m_accounts)
        if (access == account)
            return true;
    return false;
}

void Database::add_user(
    const std::string& username, const std::string& password) &
{
    Account account(username, password);

    if (!is_duplicate(account))
        m_accounts.push_back(account);
}

void Database::assign_admin(
    const std::string& username, const std::string& password) &
{
    std::string auth_user;
    std::string auth_pass;

    std::cout << "Enter administrator username: ";
    std::cin >> auth_user;
    std::cout << "Enter administrator password: ";
    std::cin >> auth_pass;

    if (m_administrator == Account(auth_user, auth_pass)) {
        Account account(username, password);
        if (!is_duplicate(account))
            m_administrator = account;
    }
}

void Database::import(const std::string& filename)
{
    std::fstream inFile(filename, std::ios_base::in);

    if (inFile.peek() == std::ifstream::traits_type::eof())
        return;

    std::string content(
        (std::istreambuf_iterator<char>(inFile)),
        std::istreambuf_iterator<char>());

    inFile.close();

    json data = json::parse(content.c_str());

    for (const auto& [key, value] : data.items()) {
        m_accounts.emplace_back(key, value);
        loaded++;
    }
}

void Database::save(const std::string& filename) const
{
    std::fstream outFile(filename, std::ios_base::out);
    json data = json::object();

    for (const auto& account : m_accounts)
        data.push_back({account.get_username(), account.get_hashed_pw()});

    outFile << std::setw(4) << data;
    outFile.close();
}

bool Database::is_duplicate(const Account& query)
{
    for (const auto& account : m_accounts)
        if (query == account)
            return true;
    return false;
}