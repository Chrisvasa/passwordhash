#include <functional>
#include <iostream>
#include <regex>

bool isValidEmail(std::string userName)
{
    std::regex pattern("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[^a-zA-Z\\d])(?=.*@).+$");
    return (userName.length() > 7 && std::regex_match(userName, pattern));
}

bool isValidPassword(std::string password)
{
    std::regex pattern("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[^a-zA-Z\\d]).+$");
    return (password.length() > 7 && std::regex_match(password, pattern));
}

std::string getValidInput(std::function<bool(std::string&)> isValid, std::string prompt)
{
    std::string input;
    while(true)
    {
        std::cout << prompt;
        if(!(std::cin >> input) || !isValid(input))
            std::cout << "Wrong input. Try again." << std::endl;
        else
            break;
    }
    return input;
}
