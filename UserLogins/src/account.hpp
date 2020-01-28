#pragma once

#if !defined(Account_header)
#define Account_header

#include <cstdlib>      // exit

#include <iostream>
#include <string>
#include <utility>      // move
#include <functional>   // function

#include "hashing.hpp"

#define USERNAME_LEN    32
#define HASHED_PW_LEN   64

class Account {
public:
    explicit Account(
        const std::string& username,
        const std::string& password,
        const std::string& algorithm = "SHA256");

    std::string get_username() const
    {return m_username;}

    std::string get_hashed_pw() const
    {return m_hashed_pw;}
    
private:
    std::string m_username;
    std::string m_hashed_pw;
    std::function<std::string(std::string)> m_algorithm;
};

inline bool operator==(const Account& lhs, const Account& rhs)
{
    return lhs.get_username() == rhs.get_username() &&
           lhs.get_hashed_pw() == rhs.get_hashed_pw();
}

#endif