#include "caching.hpp"

static unsigned char result[MD5_DIGEST_LENGTH];

static unsigned long get_size_by_fd(int fd)
{
  struct stat statbuf;

  if (fstat(fd, &statbuf) < 0)
    exit(1);
    
  return statbuf.st_size;
}

static unsigned char* calc_hash(const std::string& filename)
{
  int fd;
  unsigned long file_size;
  char* file_buffer;

  fd = open(filename.c_str(), O_RDONLY);

  if (fd < 0)
    exit(-1);

  file_size = get_size_by_fd(fd);
  file_buffer = (char*) mmap(0, file_size, PROT_READ, MAP_SHARED, fd, 0);
  
  MD5((unsigned char*) file_buffer, file_size, result);
  munmap(file_buffer, file_size);

  return result;
}

std::string Caching::get_hash(const std::string& filename)
{
  unsigned char* hash = calc_hash(filename);
  char res[MD5_DIGEST_LENGTH * 2 + 1];

  for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
    std::sprintf(&res[i * 2], "%02x", (unsigned) hash[i]);

  return std::string(res);
}

static std::ptrdiff_t count_tab(const std::string& s)
{
  std::regex pattern(R"(\{|\[)");
  
  return std::distance(
    std::sregex_iterator(s.begin(), s.end(), pattern),
    std::sregex_iterator());
}

static void reduce_tab(const std::string& s, int& n)
{
  std::regex pattern(R"(\}|\])");

  n -= std::distance(
    std::sregex_iterator(s.begin(), s.end(), pattern),
    std::sregex_iterator()
  );
}