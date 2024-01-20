#include <functional>
#include <iostream>
#include <regex>

bool isValidEmail(const std::string& userName)
{
    std::regex pattern("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[^a-zA-Z\\d])(?=.*@).+$");
    return (userName.length() > 7 && std::regex_match(userName, pattern));
}

bool isValidPassword(const std::string& password)
{
    std::regex pattern("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[^a-zA-Z\\d]).+$");
    return (password.length() > 7 && std::regex_match(password, pattern));
}

void getValidInput(std::function<bool(std::string&)> isValid, std::string& input, const std::string& prompt)
{
    while(true)
    {
        std::cout << prompt;
        if(!(std::cin >> input) || !isValid(input))
            std::cout << "Wrong input. Try again." << std::endl;
        else
            break;
    }
}