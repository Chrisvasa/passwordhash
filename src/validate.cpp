#include <regex>
#include <algorithm>
#include <cctype>
#include <string>

bool isValidEmail(const std::string& userName)
{
    std::regex pattern("^[A-Za-z]{2,}@[A-Za-z]+\\.[A-Za-z]{2,}"); // Ex: gg@gg.gg
    return (userName.length() > 7 && std::regex_match(userName, pattern));
}

bool isValidPassword(const std::string& password)
{
    std::regex pattern("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[^a-zA-Z\\d]).+$"); // Ex: Ggg1234!
    return (password.length() > 7 && std::regex_match(password, pattern));
}

bool equalsIgnoreCase(const std::string& a, const std::string& b)
{
  if(a.size() != b.size())
    return false;
  for (int i = 0; i < a.size(); i++)
    if(std::tolower(a[i]) != std::tolower(b[i]))
      return false;
  return true;
}

bool containsDigit(const std::string& str) 
{
    return std::any_of(str.begin(), str.end(), ::isdigit);
}

bool containsUppercase(const std::string& str) 
{
    return std::any_of(str.begin(), str.end(), ::isupper);
}

bool containsLowercase(const std::string& str) 
{
    return std::any_of(str.begin(), str.end(), ::islower);
}

bool containsSymbols(const std::string& str) 
{
    return std::any_of(str.begin(), str.end(), [](char c) {
        return !std::isalnum(c) && !std::isspace(c);
    });
}
