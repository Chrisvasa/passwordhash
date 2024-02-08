#include <string>
#include "usermanager.h"
#include "validate.h"
#include "hash.h"
#include "file.h"

//Handles the logic surrounding the user creation and authentication

namespace Manage 
{
    bool createUser(const std::string& userName, const std::string& password, bool secure)
    {
        if(!isValidEmail(userName))
            return false;
        
        std::optional<User> user = File::getUserFromFile(userName, password);
        if(user.has_value()) {
            std::cerr << "User already exists.." << std::endl;
            return false;
        }

        if(!isValidPassword(password)) {
            std::cerr << "Invalid password." << std::endl;
            return false;
        }

        std::string salt = Hash::generateSalt();
        std::string safeHash = Hash::hashPassword(salt + password, secure);
        std::string hash = Hash::hashPassword(password, secure);

        // Saves the "safer" user
        File::saveUserToFile(userName, salt, safeHash);
        // Saves the "unsafe" user
        File::saveUnsafeToFile(userName, hash);
        std::cout << "User was created with the following information.\n" 
            << "Username: " << userName << "\nPassword: " << safeHash << std::endl;
        return true;
    }

    bool authenticateAndLogin(const std::string& userName, const std::string& password)
    {
        std::optional<User> user = File::getUserFromFile(userName, password);
        if(!user.has_value()) {
            std::cerr << "User not found." << std::endl;
            return false;
        }
        return true;
    }
}