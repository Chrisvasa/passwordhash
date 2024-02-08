#ifndef _HASH_H_
#define _HASH_H_

#include <string>

namespace Hash
{
    std::string hashPassword(const std::string& password, bool security);
    bool isPasswordSecure(const std::string& pass);
    bool validatePassword(const std::string& pass, const std::string& salt, const std::string password);
    std::string generateSalt();
}

#endif