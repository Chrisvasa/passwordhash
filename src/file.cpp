#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <functional>
#include <string>
#include <random>
#include <openssl/evp.h>
#include "../include/file.h"
#include "../include/validate.h"
#include "../include/hash.h"


// Manages searching, reading and writing to files
namespace File
{
    // Variables to make changes a bit easier since these show up a lot in the file.
    constexpr char DELIMITER {';'};
    const std::string USERFILE {"data/users.txt"};
    const std::string UNSAFE_USERS {"data/unsafe_users.txt"};

    // Input
    bool openFile(std::ifstream& file, const std::string& path) {
        file.open(path);
        if(!file.is_open()) {
            std::cerr << "Couldn't open file: " << path << std::endl;
            return false;
        }
        return true;
    }

    // Output with standard mode set to appending text to end of file
    bool openFile(std::ofstream& file, const std::string& path, std::ios_base::openmode mode = std::ios::app) {
        file.open(path);
        if(!file.is_open()) {
            std::cerr << "Couldn't open file: " << path << std::endl;
            return false;
        }
        return true;
    }

    // Binary search that returns true or false depending on if targetValue was found
    bool binarySearchInFile(const std::string& targetVal, const std::string& path)
    {
        std::ifstream file;
        if(!openFile(file, path))
            return false;

        int start = 0;
        file.seekg(0, std::ios::end);
        int end = file.tellg();
        std::string line;

        while(start <= end)
        {
            int mid = start + (end - start) / 2;
            file.seekg(mid);

            // If not at the start of row
            if (mid != 0)
                std::getline(file, line);
            
            // Handle potential read failure
            if (!std::getline(file, line)) {
                end = mid - 1;
                file.clear();
                continue;
            }

            std::getline(file, line);
            std::istringstream iss(line);
            std::string password, hash;

            if(std::getline(iss, password, DELIMITER) && std::getline(iss, hash))
            {
                if (hash == targetVal)
                {
                    file.close();
                    return true;
                }
                else if (hash < targetVal)
                    start = mid + 1;
                else
                    end = mid - 1;
            }
        }
        file.close();
        return false;
    }

    // OVERLOADED Binary search that returns true or false depending on if targetValue was found
    // Also takes a reference to string to return a password if found
    bool binarySearchInFile(const std::string& targetVal, std::string& foundVal, const std::string& path)
    {
        std::ifstream file;
        if(!openFile(file, path))
            return false;

        int start = 0;
        file.seekg(0, std::ios::end);
        int end = file.tellg();
        std::string line;

        while(start <= end)
        {
            int mid = start + (end - start) / 2;
            file.seekg(mid);

            // If not at the start of row
            if (mid != 0)
                std::getline(file, line);

            // Handle potential read failure
            if (!std::getline(file, line)) {
                end = mid - 1;
                file.clear();
                continue;
            }

            std::getline(file, line);
            std::istringstream iss(line);
            std::string password, hash;

            if(std::getline(iss, password, DELIMITER) && std::getline(iss, hash))
            {
                if (hash == targetVal)
                {
                    foundVal = password;
                    file.close();
                    return true;
                }
                else if (hash < targetVal)
                    start = mid + 1;
                else
                    end = mid - 1;
            }
        }
        file.close();
        return false;
    }

    // Function to fetch users from a file and return said user or a nullptr
    std::optional<User> getUserFromFile(const std::string& targetUser, const std::string& pass)
    {
        std::ifstream file;
        if(!openFile(file, USERFILE))
            return std::nullopt;
        
        std::string line;
        while(std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string username, salt, password;

            if(std::getline(iss, username, DELIMITER) && std::getline(iss, salt, DELIMITER) && std::getline(iss, password))
                if(equalsIgnoreCase(username, targetUser) && Hash::validatePassword(pass, salt, password))
                    return User(username, Hash::isPasswordSecure(password));
        }
        return std::nullopt;
    }

    void saveUserToFile(const std::string& username, const std::string& salt, const std::string& pass)
    {
        std::ofstream file;
        if (openFile(file, USERFILE)) {
            file << username << DELIMITER << salt << DELIMITER << pass << std::endl;
        }
        std::cout << "User was sucessfully saved." << std::endl;
        file.close();

    }

    void saveUnsafeToFile(const std::string& username, const std::string& pass)
    {
        std::ofstream file;
        if (openFile(file, USERFILE)) {
            file << username << DELIMITER << pass << std::endl;
        }
        std::cout << "User was sucessfully saved." << std::endl;
        file.close();
    }

    /*  Opens both input & output files
        Executes passed in function (std::function)
        Replaces the original input file with the temporary output file.
    */
    void readAndWriteToFile(std::function<void(std::string&, std::ifstream&, std::ofstream&)> doTheThing, const std::string& inPath, const std::string& outPath)
    {
        std::ifstream inFile;
        std::ofstream outFile;
        if(!openFile(inFile, inPath) || !openFile(outFile, outPath))
            return;

        std::string line;
        doTheThing(line, inFile, outFile);

        inFile.close();
        outFile.close();

        std::remove(inPath.c_str());
        std::rename(outPath.c_str(), inPath.c_str());
    }

    // Uses binarySearch to find as many matching passwords as possible in given hash file
    int findMatches(const std::string& path)
    {
        std::ifstream file;
        std::string line;
        int count = 0;
        if(!openFile(file, path))
            return count;

        while(std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string pass, hash;
            if(std::getline(iss, hash))
              if (binarySearchInFile(hash))
                count++;
        }
        file.close();
        return count;
    }

    // Not very effective, since it goes line by line and makes sure each password is valid.
    // Better not use this on really large files :DDD
    void ensureValidPasswords(std::string& line, std::ifstream& inFile, std::ofstream& outFile)
    {
        std::mt19937 gen(static_cast<long unsigned int>(time(0))); 
        std::uniform_int_distribution<> distAlpha(0, 25); 
        std::uniform_int_distribution<> distNum(0, 9); 

        auto addRandomCharacter = [&](char from, char to) {
            int range = to - from + 1;
            return static_cast<char>(from + distAlpha(gen) % range);
        };

        while(std::getline(inFile, line))
        {
            if(!isValidPassword(line))
            {
                if(!containsDigit(line)) line += std::to_string(distNum(gen));
                if(!containsLowercase(line)) line += addRandomCharacter('a', 'z');
                if(!containsSymbols(line)) line += "!";
                if(!containsUppercase(line)) line.insert(line.begin(), addRandomCharacter('A', 'Z'));
            }
            outFile << line << '\n';
        }
    }

    void appendHashesToExistingPasswords(std::string& line, std::ifstream& inFile, std::ofstream& outFile)
    {
      while(std::getline(inFile, line))
      {
        outFile << line << DELIMITER;
        outFile << Hash::hashPassword(line, false) << '\n';
      }
    }
}
