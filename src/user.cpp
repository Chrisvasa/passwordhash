#include "../include/user.h"

/*  Basic User class
    Pretty much only here to store Username and their security level
*/

User::User(const std::string userName, bool secure) :
    userName(userName), secure(secure) {}

std::string User::getUserName() const { return userName; }
bool User::isSecure() const { return secure; }