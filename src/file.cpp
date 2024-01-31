#include <algorithm>
#include <iostream>
#include <regex>
#include <vector>
#include <fstream>
#include <iomanip>
#include <optional>
#include <openssl/evp.h>
#include "file.h"

namespace File
{
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
}