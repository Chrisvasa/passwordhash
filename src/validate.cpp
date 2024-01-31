#include <functional>
#include <iostream>
#include <regex>

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