#ifndef _FILE_H_
#define _FILE_H_
#include <string>
#include <optional>
#include "user.h"

namespace File
{
    std::optional<User> getUserFromFile(const std::string& targetUser);
    void saveUserToFile(User& user, const std::string& pass);
    void saveUnsafeToFile(User& user, const std::string& pass);
}

#endif