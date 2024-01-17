#include <algorithm>
#include <functional>
#include <iostream>
#include <regex>
#include <vector>

#include "validate.h"
#include "user.h"

// TEMP USER LIST <<<<< REMOVE
std::vector<User> userList;
// TEMP USER LIST <<<<< REMOVE

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
    std::function<bool(std::string)> userFunc = isValidEmail;
    std::function<bool(std::string)> passFunc = isValidPassword;

    std::string userName = getValidInput(userFunc, "Username: ");
    std::string password = getValidInput(passFunc, "Password: ");

    User user(userName, password);
    userList.push_back(user);
    std::cout << "User was created with the following information.\n" 
    << "Username: " << userName << "\nPassword: " << password << std::endl;
}

int main(int argc, char const *argv[])
{
    createUser();
    createUser();

    for(auto& user : userList)
    {
        std::cout << user.getUserName() << "\n";
    }
    
    return 0;
}