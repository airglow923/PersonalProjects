#include "account.hpp"

Account::Account(
    const std::string& username,
    const std::string& password,
    const std::string& algorithm)
    : m_username(username)
{
    if (algorithm == "MD5") {
        m_algorithm = Hashing::get_MD5(password);
    } else if (algorithm == "SHA256") {
        m_algorithm = Hashing::get_SHA256(password);
    } else if (algorithm == "PLAIN") {
        m_hashed_pw = password;
    } else {
        std::cerr << "Invalid algorithm.\n";
        exit(EXIT_FAILURE);
    }
}

Account::Account(const Account& account)
    : m_username(account.m_username), m_hashed_pw(account.m_hashed_pw)
{
}

Account::Account(Account&& account)
    : m_username(std::move(account.m_username)),
        m_hashed_pw(std::move(account.m_hashed_pw))
{
}