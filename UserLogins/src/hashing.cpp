#include "hashing.hpp"

static unsigned char md5_result[MD5_DIGEST_LENGTH];
static unsigned char sha256_result[SHA256_DIGEST_LENGTH];

std::string Hashing::get_MD5(const std::string& s)
{
  char res[MD5_DIGEST_LENGTH * 2 + 1];

  MD5((unsigned char*) s.c_str(), s.size(), md5_result);
  stringify(res, MD5_DIGEST_LENGTH, md5_result);
  
  return std::string(res);
}

std::string Hashing::get_SHA256(const std::string& s)
{
  char res[SHA256_DIGEST_LENGTH * 2  + 1];

  if (calc_SHA256(s)) {
    stringify(res, SHA256_DIGEST_LENGTH, sha256_result);
    return std::string(res);
  }

  return nullptr;
}

bool Hashing::calc_SHA256(const std::string& s)
{
  SHA256_CTX context;

  if (!SHA256_Init(&context))
    return false;

  if (!SHA256_Update(&context, (unsigned char*) s.c_str(), s.size()))
    return false;

  if (!SHA256_Final(sha256_result, &context))
    return false;

  return true;
}

void Hashing::stringify(char* buf, std::size_t len, unsigned char* input)
{
  for (std::size_t i = 0; i < len; i++)
    std::sprintf(&buf[i * 2], "%02X", (unsigned) input[i]);
}

unsigned Hashing::F(unsigned x, unsigned y, unsigned z)
{
  return (x & y) | (~x & z);
}

unsigned Hashing::G(unsigned x, unsigned y, unsigned z)
{
  return (x & y) | (y & ~z);
}

unsigned Hashing::H(unsigned x, unsigned y, unsigned z)
{
  return x ^ y ^ z;
}

unsigned Hashing::I(unsigned x, unsigned y, unsigned z)
{
  return y ^ (x | ~z);
}