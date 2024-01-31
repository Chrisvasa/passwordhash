#ifndef _USERMANAGER_H_
#define _USERMANAGER_H_

#include "user.h"

void createUser();
bool authenticateUser(User& user, std::string& password);
bool authenticateAndLogin(std::string userName, std::string password);

#endif