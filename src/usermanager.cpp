#include <optional>
#include <string>
#include "usermanager.h"
#include "validate.h"
#include "hash.h"
#include "file.h"

void createUser()
{   
    std::string userName {};
    std::string password {};
    getValidInput(isValidEmail, userName, "Username: ");
    std::optional<User> user = File::getUserFromFile(userName);
    if(user.has_value())
    {
        std::cout << "User already exists.." << std::endl;
        return;
    }
    getValidInput(isValidPassword, password, "Password: ");
    std::string salt = Hash::generateSalt();
    std::string safeHash = Hash::hashPassword(salt + password);
    std::string hash = Hash::hashPassword(password);
    // Saves the "safer" user
    User newUser(userName, salt, safeHash);
    File::saveUserToFile(newUser);
    // Saves the "unsafe" user
    newUser.setPassword(hash); // Changes password to unsalted hash
    File::saveUnsafeToFile(newUser);
    std::cout << "User was created with the following information.\n" 
        << "Username: " << userName << "\nPassword: " << safeHash << std::endl;
}

bool authenticateUser(User& user)
{
    std::string password {};
    getValidInput(isValidPassword, password, "Password: ");
    std::string hash = Hash::hashPassword(user.getSalt() + password);
    return user.verifyLogin(user.getUserName(), hash);
}

bool login()
{
    std::string userName {};
    getValidInput(isValidEmail, userName, "Username: ");
    std::optional<User> user = File::getUserFromFile(userName);
    if(!user) {
        std::cout << "User not found." << std::endl;
        return false;
    }
    return (authenticateUser(user.value()));
}