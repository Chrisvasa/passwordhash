#include <algorithm>
#include <functional>
#include <iostream>
#include <regex>
#include <vector>
#include <fstream>

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
    std::string userName = getValidInput(isValidEmail, "Username: ");
    std::string password = getValidInput(isValidPassword, "Password: ");
    
    // TEMP - User check list 
    for(auto& user : userList)
    {
        if(user.getUserName() == userName)
        {
            std::cout << "User already exists.." << std::endl;
            return;
        }
    }

    User user(userName, password);
    userList.push_back(user);
    std::cout << "User was created with the following information.\n" 
    << "Username: " << userName << "\nPassword: " << password << std::endl;
}


bool login()
{
    std::string userName = getValidInput(isValidEmail, "Username: ");
    std::string password = getValidInput(isValidPassword, "Password: ");

    for(auto& user : userList)
        if(user.verifyLogin(userName,password))
            return true;
    return false;
}

void saveUsersToFile()
{
    std::string filename = "data/users.txt";
    std::ofstream file;

    file.open(filename);

    int size = userList.size();
    std::cout << size << std::endl;
    for(int i = 0; i < size; i++)
    {
        file << userList[i].getUserName() << ";" << userList[i].getPassword();
        if(i < size - 1)
            file << '\n';
    }

    // if(!file)
    // {
    //     std::cerr << "Unable to open file." << std::endl;
    // }
}

int main(int argc, char const *argv[])
{
    createUser();
    createUser();

    // TEMP - prints created users
    for(auto& user : userList)
    {
        std::cout << user.getUserName() << "\n";
    }

    // TEMP - User login
    if(login())
        std::cout << "Login successful!" << std::endl;
    else
        std::cout << "Invalid Username or Password." << std::endl;
    
    saveUsersToFile();
    
    return 0;
}