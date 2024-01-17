#ifndef _USER_H_
#define _USER_H_
#include <iostream>

class User {
    std::string userName;
    std::string passWord;
public:
    User(std::string uName, std::string pWord);
    std::string getUserName();
    // void setUserName(std::string uName) { userName = uName; }
    std::string getPassword();
    bool setPassword(std::string pass);
    bool verifyLogin(std::string user, std::string pass);
};

#endif