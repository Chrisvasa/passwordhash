#include "../include/user.h"

User::User(const std::string userName, bool secure) :
    userName(userName), secure(secure) {}

std::string User::getUserName() const { return userName; }
bool User::isSecure() const { return secure; }