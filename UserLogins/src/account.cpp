#include "account.hpp"

Account::Account(
    const std::string& username,
    const std::string& password)
    : m_username(username)
{
    m_algorithm = Hashing::get_SHA256;
    m_hashed_pass = m_algorithm(password);
}

Account::Account(
    std::string&& username,
    std::string&& password)
    : m_username(username)
{
    m_algorithm = Hashing::get_SHA256;
    m_hashed_pass = m_algorithm(std::move(password));
}

Account::Account(
    const std::string& username,
    const std::string& password,
    const std::string& algorithm)
    : m_username(username)
{
    if (algorithm == "MD5")
        m_algorithm = Hashing::get_MD5;
    else if (algorithm == "SHA256")
        m_algorithm = Hashing::get_SHA256;

    m_hashed_pass = m_algorithm(password);
}

Account::Account(
    std::string&& username,
    std::string&& password,
    std::string&& algorithm)
    : m_username(std::move(username))
{
    if (algorithm == "MD5")
        m_algorithm = Hashing::get_MD5;
    else if (algorithm == "SHA256")
        m_algorithm = Hashing::get_SHA256;

    m_hashed_pass = m_algorithm(std::move(password));
}