#ifndef _USER_H_
#define _USER_H_
#include <iostream>

class User {
    std::string userName;
    std::string password;
public:
    User(const std::string userName, std::string password);
    std::string getUserName() const;
    std::string getPassword() const;
    bool setPassword(std::string pass);
    bool verifyLogin(const std::string& user,const std::string& pass);
};

#endif