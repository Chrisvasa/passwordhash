#include "user.h"
#include "validate.h"

User::User(const std::string userName, std::string password)
{
    this->userName = userName;
    this->password = password;
}

std::string User::getUserName() const { return userName; }
std::string User::getPassword() const { return password; }
bool User::setPassword(const std::string pass) 
{ 
    if(isValidPassword(pass))
    {
        password = pass; 
        std::cout << "Password was changed." << std::endl;
        return true;
    }
    return false;
}
bool User::verifyLogin(const std::string& user, const std::string& pass)
{
    return (user == userName && pass == password);
}