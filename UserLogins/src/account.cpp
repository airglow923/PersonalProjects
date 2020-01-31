#include "account.hpp"

Account::Account(
    const std::string& username,
    const std::string& password,
    const std::string& algorithm)
    : m_username(username), m_algorithm(algorithm)
{
    if (m_algorithm == "MD5") {
        m_hashed_pw = Hashing::get_MD5(password);
    } else if (m_algorithm == "SHA256") {
        m_hashed_pw = Hashing::get_SHA256(password);
    } else if (m_algorithm == "PLAIN") {
        m_hashed_pw = password;
        m_algorithm = "NULL";
    } else {
        std::cerr << "Invalid algorithm.\n";
        exit(EXIT_FAILURE);
    }
}

Account::Account(const Account& account)
    : m_username(account.m_username), m_hashed_pw(account.m_hashed_pw),
        m_algorithm(account.m_algorithm)
{
}

Account::Account(Account&& account)
    : m_username(std::move(account.m_username)),
        m_hashed_pw(std::move(account.m_hashed_pw)),
        m_algorithm(std::move(account.m_algorithm))
{
}