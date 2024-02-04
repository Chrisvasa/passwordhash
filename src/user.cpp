#include "../include/user.h"

User::User(const std::string userName, const std::string salt, std::string password) :
    userName(userName), salt(salt), password(password) {}

std::string User::getUserName() const { return userName; }
std::string User::getSalt() const { return salt; }
bool User::isSecure() const { return password.length() > 32 ? true : false; } // Checks if MD5 or SHA256

bool User::verifyLogin(const std::string& user, const std::string& pass)
{
    return (user == userName && pass == password);
}