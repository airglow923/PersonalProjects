#include "hashing.hpp"

static unsigned char md5_result[MD5_DIGEST_LENGTH];
static unsigned char sha256_result[SHA256_DIGEST_LENGTH];

std::string Hashing::get_MD5(const std::string& s)
{
  MD5((unsigned char*) s.c_str(), s.size(), md5_result);
  char res[MD5_DIGEST_LENGTH * 2 + 1];

  for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
    std::sprintf(&res[i * 2], "%02X", (unsigned) md5_result[i]);

  res[MD5_DIGEST_LENGTH * 2] = '\0';
  
  return std::string(res);
}

std::string Hashing::get_SHA256(const std::string& s)
{
  char res[SHA256_DIGEST_LENGTH * 2  + 1];

  if (calc_SHA256(s)) {
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
      std::sprintf(&res[i * 2], "%02X", (unsigned) sha256_result[i]);

    res[SHA256_DIGEST_LENGTH * 2] = '\0';

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