#ifndef _HASH_H_
#define _HASH_H_

#include <string>

namespace Hash
{
    std::string HashPassword(const std::string& password, bool security);
    std::string GenerateSalt();
}

#endif