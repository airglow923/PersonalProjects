#pragma once

#if !defined(Account_header)
#define Account_header

#include <cstdlib>      // exit

#include <iostream>
#include <string>
#include <utility>      // move
#include <functional>   // function

#include "hashing.hpp"

#define NAME_MAX_LEN    32
#define HASH_MAX_LEN    64
#define ALGO_MAX_LEN    10

class Account {
public:
    explicit Account(
        const std::string& username,
        const std::string& password,
        const std::string& algorithm = "SHA256");
    Account(const Account&);
    Account(Account&&);

    std::string get_username() const
    {return m_username;}

    std::string get_hashed_pw() const
    {return m_hashed_pw;}

    std::string get_algorithm() const
    {return m_algorithm;}
    
private:
    std::string m_username;
    std::string m_hashed_pw;
    std::string m_algorithm;
};

inline bool operator==(const Account& lhs, const Account& rhs)
{
    return lhs.get_username() == rhs.get_username() &&
           lhs.get_hashed_pw() == rhs.get_hashed_pw();
}

#endif