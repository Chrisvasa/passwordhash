#include "user.h"
#include "validate.h"

User::User(std::string uName, std::string pWord)
{
    userName = uName;
    passWord = pWord;
}

std::string User::getUserName() { return userName; }
// void setUserName(std::string uName) { userName = uName; }
std::string User::getPassword() { return passWord; }
bool User::setPassword(std::string pass) 
{ 
    if(pass.length() > 7 && isValidPassword(pass))
    {
        passWord = pass; 
        std::cout << "Password was changed." << std::endl;
        return true;
    }
    return false;
}
bool User::verifyLogin(std::string user, std::string pass)
{
    return (user == userName && pass == passWord);
}