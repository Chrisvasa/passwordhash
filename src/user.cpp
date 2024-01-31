#include "../include/user.h"
#include "../include/validate.h"

User::User(const std::string userName, const std::string salt, std::string password) :
    userName(userName), salt(salt), password(password) {}

std::string User::getUserName() const { return userName; }
std::string User::getSalt() const { return salt; }
bool User::isSecure() const { return password.length() > 32 ? true : false; }

// void User::setPassword(std::string pass) 
// { 
//     password = pass; 
//     std::cout << "Password was changed." << std::endl;
// }

bool User::verifyLogin(const std::string& user, const std::string& pass)
{
    return (user == userName && pass == password);
}