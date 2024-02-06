#include <regex>
#include <algorithm>
#include <cctype>

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
