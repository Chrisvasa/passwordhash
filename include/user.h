#ifndef _USER_H_
#define _USER_H_
#include <iostream>

class User {
    std::string userName;
    bool secure = false;
public:
    User(const std::string userName, bool secure);
    std::string getUserName() const;
    bool isSecure() const;
};

#endif
