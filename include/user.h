#ifndef _USER_H_
#define _USER_H_
#include <iostream>

class User {
    std::string userName;
    std::string password;
    std::string salt;
public:
    User(const std::string userName, const std::string salt, std::string password);
    std::string getUserName() const;
    std::string getSalt() const;
    bool isSecure() const;
    // void setPassword(std::string pass);
    bool verifyLogin(const std::string& user,const std::string& pass);
};

#endif
