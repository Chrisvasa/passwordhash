#ifndef _USERMANAGER_H_
#define _USERMANAGER_H_

#include "user.h"

bool createUser(const std::string& userName, const std::string& password);
bool authenticateUser(User& user, std::string& password);
bool authenticateAndLogin(std::string userName, std::string password);

#endif