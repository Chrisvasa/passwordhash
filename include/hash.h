#ifndef _HASH_H_
#define _HASH_H_

#include <string>

namespace Hash
{
    std::string hashPassword(const std::string& password, bool security);
    std::string generateSalt();
}

#endif