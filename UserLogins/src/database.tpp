#include "database.hpp"

template<typename... Args>
bool Database::authenticate(Args&&... args) const &
{
    Account access(std::forward<Args>(args)...);

    for (const auto& account : m_accounts)
        if (access == account)
            return true;
    return false;
}

template<typename... Args>
void Database::add_user(Args&&... args) &
{
    Account account(std::forward<Args>(args)...);

    if (!is_duplicate(account))
        m_accounts.push_back(account);
}

template<typename... Args>
void Database::assign_admin(Args&&... args) &
{
    std::string auth_user;
    std::string auth_pass;

    std::cout << "Enter administrator username: ";
    std::cin >> auth_user;
    std::cout << "Enter administrator password: ";
    std::cin >> auth_pass;

    if (m_administrator == Account(auth_user, auth_pass)) {
        Account account(std::forward<Args>(args)...);
        if (!is_duplicate(account))
            m_administrator = account;
    } else {
        std::cerr << "Access denied.\n";
    }
}