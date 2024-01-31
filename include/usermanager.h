#ifndef _USERMANAGER_H_
#define _USERMANAGER_H_

#include "user.h"

void createUser();
bool authenticateUser(User& user);
bool login();

#endif