#include <algorithm>
#include <chrono>
#include <iostream>
#include <regex>
#include <vector>
#include <fstream>
#include <iomanip>
#include <optional>
#include <openssl/evp.h>

#include "validate.h"
#include "user.h"

std::string hashPassword(const std::string& password);
std::optional<User> getUserFromFile(const std::string& targetUser);
bool isEmptyFile(const std::string& filename);
std::string generateSalt();
void saveUserToFile(User user);
void saveUnsafeToFile(User user);
void createUser();
bool login();
void tempMenu(std::vector<std::string> menuItems);

int main(int argc, char const *argv[])
{
    srand(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
    tempMenu({"Login", "Create Account", "Exit"});
    return 0;
}

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
        std::string username, salt, password;

        if(std::getline(iss, username, ';') && std::getline(iss, salt, ';') && std::getline(iss, password))
        {
            if(username == targetUser)
            {
                return User(username, salt, password);
            }
        }
    }
    
    return std::nullopt;
}

bool isEmptyFile(const std::string& filename)
{
    std::ifstream file(filename);
    if(!file)
    {
        std::cerr << "File not found." << std::endl;
        exit(EXIT_FAILURE);
    }

    return file.peek() == std::ifstream::traits_type::eof();
}

std::string generateSalt()
{
    char s[16];
    for (int i = 0; i < 16; i++)
    {
        s[i] = (48 + (rand() % 75));
    }
    
    s[15] = '\0';
    return s;
}

void saveUserToFile(User user)
{
    std::string filename = "data/users.txt";
    std::ofstream file;

    file.open(filename, std::ios::app);
    if(!isEmptyFile(filename))
        file << std::endl;
    file << user.getUserName() << ";" << user.getSalt() << ";" << user.getPassword();
    file.close();

    std::cout << "User was sucessfully saved." << std::endl;
}

void saveUnsafeToFile(User user)
{
    std::string filename = "data/unsafe_users.txt";
    std::ofstream file;

    file.open(filename, std::ios::app);
    if(!isEmptyFile(filename))
        file << std::endl;
    file << user.getUserName() << ";" << user.getPassword();
    file.close();

    std::cout << "User was sucessfully saved." << std::endl;
}

void createUser()
{   
    std::string userName {};
    std::string password {};

    getValidInput(isValidEmail, userName, "Username: ");
    std::optional<User> user = getUserFromFile(userName);
    if(user.has_value())
    {
        std::cout << "User already exists.." << std::endl;
        return;
    }

    getValidInput(isValidPassword, password, "Password: ");
    std::string salt = generateSalt();
    std::string safeHash = hashPassword(salt + password);
    std::string hash = hashPassword(password);

    // Saves the "safer" user
    User newUser(userName, salt, safeHash);
    saveUserToFile(newUser);
    // Saves the "unsafe" user
    newUser.setPassword(hash); // Changes password to unsalted hash
    saveUnsafeToFile(newUser);
    std::cout << "User was created with the following information.\n" 
        << "Username: " << userName << "\nPassword: " << safeHash << std::endl;
}

bool authenticateUser(User& user)
{
    std::string password {};
    getValidInput(isValidPassword, password, "Password: ");
    std::string hash = hashPassword(user.getSalt() + password);
    return user.verifyLogin(user.getUserName(), hash);
}

bool login()
{
    std::string userName {};
    getValidInput(isValidEmail, userName, "Username: ");
    std::optional<User> user = getUserFromFile(userName);
    if(!user) {
        std::cout << "User not found." << std::endl;
        return false;
    }

    return (authenticateUser(user.value()));
}

void tempMenu(std::vector<std::string> menuItems)
{
    while(true)
    {
        for(int i = 0; i < (int)menuItems.size(); i++)
        {
            std::cout << i + 1 << ". " << menuItems[i] << '\n';
        }

        int input;
        if(!(std::cin >> input))
        {
            std::cout << "Wrong input." << std::endl;
            return;
        }

        switch(input)
        {
            case 1:
                if(login())
                    std::cout << "Login successful!" << std::endl;
                else
                    std::cout << "Invalid Username or Password." << std::endl;
                break;
            case 2:
                createUser();
                break;
            default:
                return;
        }
    }
}