#ifndef _HASH_H_
#define _HASH_H_

#include <string>

namespace Hash
{
    std::string hashPassword(const std::string& password);
    std::string generateSalt();
}

#endif