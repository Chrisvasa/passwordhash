#include <optional>
#include <string>
#include "usermanager.h"
#include "validate.h"
#include "hash.h"
#include "file.h"

bool createUser(const std::string& userName, const std::string& password, bool secure)
{
    if(!isValidEmail(userName))
        return false;
    
    std::optional<User> user = File::getUserFromFile(userName);
    if(user.has_value()) {
        std::cout << "User already exists.." << std::endl;
        return false;
    }

    if(!isValidPassword(password)) {
        std::cout << "Invalid password." << std::endl;
        return false;
    }

    std::string salt = Hash::generateSalt();
    std::string safeHash = Hash::hashPassword(salt + password, secure);
    std::string hash = Hash::hashPassword(password, secure);

    // Saves the "safer" user
    User newUser(userName, salt, safeHash);
    File::saveUserToFile(newUser, safeHash);
    // Saves the "unsafe" user
    File::saveUnsafeToFile(newUser, hash);
    std::cout << "User was created with the following information.\n" 
        << "Username: " << userName << "\nPassword: " << safeHash << std::endl;
    return true;
}

bool authenticateUser(User& user, std::string& password, bool secure)
{
    std::string hash = Hash::hashPassword(user.getSalt() + password, secure);
    return user.verifyLogin(user.getUserName(), hash);
}

bool authenticateAndLogin(std::string userName, std::string password)
{
    std::optional<User> user = File::getUserFromFile(userName);
    if(!user) {
        std::cout << "User not found." << std::endl;
        return false;
    }
    return (authenticateUser(user.value(), password, user.value().isSecure()));
}