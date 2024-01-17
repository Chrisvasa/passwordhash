#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
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

/*1. Skapa användare**  
Vid skapa användare ska du:
- fråga om användarnamn och lösenord
- Användarnamn = epostadress (validera)
- Kontrollera så användaren inte redan finns
- Alla lösenord måste uppfylla följande regelverk
    * minst 8 tecken
    * minst en stor bokstav
    * minst en liten bokstav  
    * minst en siffra  
    * minst ett specialtecken */
void createUser()
{   
    std::vector<std::function<bool(std::string)>> fList;
    fList.push_back(isValidEmail);
    fList.push_back(isValidPassword);
    std::string userName = getValidInput(fList[0], "Username: ");
    std::string password = getValidInput(fList[1], "Password: ");

    std::cout << "Username: " << userName << "\nPassword: " << password << std::endl;
}

int main(int argc, char const *argv[])
{
    createUser();
    
    return 0;
}