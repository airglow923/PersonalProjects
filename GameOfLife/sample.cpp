#include <iostream>
#include <utility>
#include <string>

void print_str(const char* str)
{
	std::cout << str;
}

void print_str(const std::string& str)
{
	std::cout << str;
}

template <typename... Args>
void print_str(const char* str, Args&&... args)
{
	print_str(str);
	print_str(std::forward<Args>(args)...);
}

int main()
{
	std::string s("iaojfpiaosdjf pajd");
	print_str("Hello,", " Warudo!", "Yo", s);
}