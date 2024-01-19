#include <algorithm>
#include <iostream>
#include <regex>
#include <vector>
#include <fstream>
#include <iomanip>
#include <optional>
#include <openssl/evp.h>

#include "validate.h"
#include "user.h"

std::string hashPassword(const std::string& password)
{
    EVP_MD_CTX* mdctx; // Pointer to the digest context
    const EVP_MD* md; // Pointer to the digest method (MD5 in this case)
    unsigned char md_value[EVP_MAX_MD_SIZE]; // Array to store the hash result
    unsigned int md_len, i;

    md = EVP_md5(); // selects the MD5 algorithm.
    mdctx = EVP_MD_CTX_new(); //creates a context for the digest.
    EVP_DigestInit_ex(mdctx, md, NULL); // initializes the digest context.
    EVP_DigestUpdate(mdctx, password.c_str(), password.size()); // hashes the data.
    EVP_DigestFinal_ex(mdctx, md_value, &md_len); // retrieves the final digest.
    EVP_MD_CTX_free(mdctx); // cleans up the context.

    std::stringstream ss;
    for(i = 0; i < md_len; i++)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(md_value[i]);
    }

    return ss.str();
}

std::optional<User> getUserFromFile(const std::string& targetUser)
{
    std::ifstream file("data/users.txt");
    std::string line;

    if(!file.is_open()) 
    {
        std::cerr << "Unable to open file" << std::endl;
        return std::nullopt;
    }

    while(std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string username, password;

        if(std::getline(iss, username, ';') && std::getline(iss, password))
        {
            if(username == targetUser)
            {
                return User(username, password);
            }
        }
    }
    
    return std::nullopt;
}

int getUserCount(const std::string& filename)
{
    std::ifstream file("data/users.txt");
    std::string line;
    int count = 0;

    if(!file.is_open()) 
    {
        std::cerr << "Unable to open file" << std::endl;
        exit(EXIT_FAILURE);
    }

    while(std::getline(file, line))
    {
        count++;
    }

    return count;
}

void saveUserToFile(User user)
{
    std::string filename = "data/users.txt";
    std::ofstream file;
    int userAmount = getUserCount(filename);

    file.open(filename, std::ios::app);
    if(userAmount > 0)
        file << std::endl;
    file << user.getUserName() << ";" << user.getPassword();
    file.close();

    std::cout << "User was sucessfully saved." << std::endl;
}

void createUser()
{   
    std::string userName = getValidInput(isValidEmail, "Username: ");
    std::string password = getValidInput(isValidPassword, "Password: ");
    std::string hash = hashPassword(password);
    
    std::optional<User> user = getUserFromFile(userName);
    if(user.has_value())
    {
        std::cout << "User already exists.." << std::endl;
        return;
    }

    User newUser(userName, hash);
    saveUserToFile(newUser);
    std::cout << "User was created with the following information.\n" 
        << "Username: " << userName << "\nPassword: " << hash << std::endl;
}

bool login()
{
    std::string userName = getValidInput(isValidEmail, "Username: ");
    std::string password = getValidInput(isValidPassword, "Password: ");
    std::string hash = hashPassword(password);

    std::optional<User> user = getUserFromFile(userName);
    if(user.has_value())
    {
        if(user.value().verifyLogin(userName,hash))
            return true;
    }
    return false;

    // 1. Greet <-- Greet users email ("Hello xxx@xxx.se")
    // 2. Logout
}

int main(int argc, char const *argv[])
{
    createUser();

    // TEMP - User login
    if(login())
        std::cout << "Login successful!" << std::endl;
    else
        std::cout << "Invalid Username or Password." << std::endl;
    return 0;
}