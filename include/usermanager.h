#ifndef _USERMANAGER_H_
#define _USERMANAGER_H_

#include "user.h"

namespace Manage
{
    bool createUser(const std::string& userName, const std::string& password, bool secure);
    bool authenticateAndLogin(const std::string& userName, const std::string& password);
}

#endif