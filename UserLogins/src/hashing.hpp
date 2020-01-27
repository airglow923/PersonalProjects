#pragma once

#if !defined(Hashing_header)
#define Hashing_header

#include <cstdio>
#include <cstdlib>          // sprintf

#include <iostream>
#include <string>

#if defined(__linux__) || defined(__CYGWIN__)
#include <openssl/md5.h>    // MD5
#include <openssl/sha.h>    // SHA
#endif

class Hashing {
public:
    static std::string get_MD5(const std::string&);
    static std::string get_SHA256(const std::string&);

private:
    static bool calc_SHA256(const std::string&);
    static void stringify(char*, std::size_t, unsigned char*);
    static unsigned F(unsigned, unsigned, unsigned);
    static unsigned G(unsigned, unsigned, unsigned);
    static unsigned H(unsigned, unsigned, unsigned);
    static unsigned I(unsigned, unsigned, unsigned);
};

#endif