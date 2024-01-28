#include "user.h"
#include "validate.h"

User::User(const std::string userName, const std::string salt, std::string password)
{
    this->userName = userName;
    this->salt = salt;
    this->password = password;
}

std::string User::getUserName() const { return userName; }
std::string User::getPassword() const { return password; }
std::string User::getSalt() const { return salt; }
void User::setPassword(std::string pass) 
{ 
    password = pass; 
    std::cout << "Password was changed." << std::endl;
}
bool User::verifyLogin(const std::string& user, const std::string& pass)
{
    return (user == userName && pass == password);
}