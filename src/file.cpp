#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <cctype>
#include <functional>
#include <string>
#include <openssl/evp.h>
#include "../include/file.h"
#include "../include/validate.h"
#include "../include/hash.h"


// Manages searching, reading and writing to files
namespace File
{
    // Variables to make changes a bit easier since these show up a lot in the file.
    const std::string USERFILE {"data/users.txt"};
    const std::string UNSAFE_USERS {"data/unsafe_users.txt"};
    constexpr char DELIMITER {';'};

    // Binary search that returns true or false depending on if targetValue was found
    bool binarySearchInFile(const std::string& targetVal, const std::string& path)
    {
        std::ifstream file(path);
        if(!file.is_open())
        {
            std::cerr << "Couldn't open file" << std::endl;
            return false;
        }

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
        std::ifstream file(path);
        if(!file.is_open())
        {
            std::cerr << "Couldn't open file" << std::endl;
            return false;
        }

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
        std::ifstream file(USERFILE);
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

            if(std::getline(iss, username, DELIMITER) && std::getline(iss, salt, DELIMITER) && std::getline(iss, password))
            {
                if(equalsIgnoreCase(username, targetUser) && Hash::validatePassword(pass, salt, password))
                {
                    return User(username, Hash::isPasswordSecure(password));
                }
            }
        }

        return std::nullopt;
    }

    void saveUserToFile(const std::string& username, const std::string& salt, const std::string& pass)
    {
        std::ofstream file;

        file.open(USERFILE, std::ios::app);
        file << username << DELIMITER << salt << DELIMITER << pass << std::endl;
        file.close();

        std::cout << "User was sucessfully saved." << std::endl;
    }

    void saveUnsafeToFile(const std::string& username, const std::string& pass)
    {
        std::ofstream file;

        file.open(UNSAFE_USERS, std::ios::app);
        file << username << DELIMITER << pass << std::endl;;
        file.close();

        std::cout << "User was sucessfully saved." << std::endl;
    }

    /*  Opens both input & output files
        Executes passed in function (std::function)
        Replaces the original input file with the temporary output file.
    */
    void readAndWriteToFile(std::function<void(std::string&, std::ifstream&, std::ofstream&)> doTheThing, const std::string& inPath, const std::string& outPath)
    {
        std::ifstream inFile(inPath);
        std::ofstream outFile(outPath);
        std::string line;

        if(!inFile.is_open() || !outFile.is_open()) 
        {
            std::cerr << "Unable to open the file" << std::endl;
            return;
        }

        doTheThing(line, inFile, outFile);

        inFile.close();
        outFile.close();

        std::remove(inPath.c_str());
        std::rename(outPath.c_str(), inPath.c_str());
    }

    // Uses binarySearch to find as many matching passwords as possible in given hash file
    int findMatches(const std::string& path)
    {
        std::ifstream file(path);
        std::string line;
        int count = 0;
        if(!file.is_open())
        {
            std::cerr << "Unable to open file." << std::endl;
            return count;
        }

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

    // REMOVE DONT WORK WITH VECTORS STUPID
    std::vector<std::pair<std::string, std::string>> fillPairFromFile(std::ifstream& file)
    {
        std::vector<std::pair<std::string, std::string>> lines;
        std::string line;

        std::cout << "Arrived at vector" << std::endl;
        while(std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string pass, hash;
            if(std::getline(iss, pass, ';') && std::getline(iss, hash))
            {
                std::pair<std::string, std::string> row { pass, hash };
                lines.push_back(row);
            }
        }

        std::cout << "Done with filling vector" << std::endl;
        return lines;
    }

    void sortTextByHash(std::string& line, std::ifstream& inFile, std::ofstream& outFile)
    {
        std::vector<std::pair<std::string, std::string>> pairs = fillPairFromFile(inFile);
        std::sort(pairs.begin(), pairs.end(), [] (std::pair<std::string, std::string>& v1, std::pair<std::string, std::string>& v2) {
            return v1.second < v2.second;
        });

        for(auto& pair : pairs)
            outFile << pair.first << ';' << pair.second << '\n';
    }

    // REWRITE THIS ABOMINATION
    void ensureValidPasswords(std::string& line, std::ifstream& inFile, std::ofstream& outFile)
    {
        while(std::getline(inFile, line))
        {
            if(line.length() > 7)
            {
                if(!isValidPassword(line))
                {
                    if(!containsDigit(line))
                    {
                        line += "123";
                    }
                    if(!containsLowercase(line))
                    {
                        char randLower = ('a' + rand() % 26);
                        line += randLower;
                    }
                    if(!containsSymbols(line))
                    {
                        line += "!";
                    }
                    if(!containsUppercase(line))
                    {
                        if(!containsDigit(line.substr(0,1)))
                            line[0] = std::toupper(line[0]);
                        else
                        {
                            char randUpper = ('A' + rand() % 26);
                            line += randUpper;
                        }
                    }  
                    outFile << line << '\n';
                }
                else
                {
                    outFile << line << '\n';
                }
            }
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
