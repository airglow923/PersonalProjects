#include "database.hpp"

Database::Database()
    : m_administrator(Account("admin", "admin"))
{
}

Database::Database(const std::string& username, const std::string& password)
    : m_administrator(Account(username, password))
{
}

Database::Database(std::string&& username, std::string&& password)
    : m_administrator(Account(std::move(username), std::move(password)))
{
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
    std::string&& username, std::string&& password) const &&
{
    Account access(
        std::move(username), std::move(password));

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

bool Database::authenticate(
    std::string&& username,
    std::string&& password,
    std::string&& algorithm) const &&
{
    Account access(
        std::move(username), std::move(password), std::move(algorithm));

    for (const auto& account : m_accounts)
        if (access == account)
            return true;
    return false;
}

void Database::add_user(
    const std::string& username, const std::string& password) &
{
    m_accounts.emplace_back(username, password);
}

void Database::add_user(
    std::string&& username, std::string&& password) &&
{
    m_accounts.emplace_back(std::move(username), std::move(password));
}

void Database::assign_admin(
    const std::string& username, const std::string& password) &
{
    m_administrator = Account(username, password);
}

void Database::assign_admin(
    std::string&& username, std::string&& password) &&
{
    m_administrator = Account(std::move(username), std::move(password));
}

void Database::import()
{
    import("config.json");
}

void Database::import(const std::string& filename)
{

}

void Database::save() const
{
    save_as("config.json");
}

void Database::save_as(const std::string& filename) const
{
    std::ofstream outFile(filename);
    json data;

    outFile << data;
    outFile.close();
}