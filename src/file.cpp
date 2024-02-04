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


// Manages all file access functions
namespace File
{
    const std::string USERFILE {"data/users.txt"};
    const std::string UNSAFE_USERS {"data/unsafe_users.txt"};
    constexpr char DELIMITER {';'};


    bool binarySearchInFile(const std::string& path, const std::string& targetVal)
    {
        std::ifstream file(path);
        if(!file.is_open())
        {
            std::cout << "Couldn't open file" << std::endl;
            return 1;
        }

        int start = 0;
        file.seekg(0, std::ios::end);
        int end = file.tellg();
        std::string line;

        while(start <= end)
        {
            int mid = start + (end - start) / 2;
            file.seekg(mid);

            if (mid != 0) { // If not at the start of the file
                std::getline(file, line); // Read and discard partial line if mid is in the middle of a line
            }

            std::getline(file, line);
            std::istringstream iss(line);
            std::string username, password;

            std::cout << line << std::endl;

            if(std::getline(iss, username, ';') && std::getline(iss, password))
            {
                if (password == targetVal || username == targetVal)
                {
                    std::cout << "Found: " << username << " with the password: " << password << std::endl;
                    return true;
                }
                else if (password < targetVal)
                    start = mid + 1;
                else
                    end = mid - 1;
            }
        }

        return false;
    }

    std::optional<User> getUserFromFile(const std::string& targetUser)
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
                if(username == targetUser)
                {
                    return User(username, salt, password);
                }
            }
        }
        
        return std::nullopt;
    }

    void saveUserToFile(User& user, const std::string& pass)
    {
        std::ofstream file;

        file.open(USERFILE, std::ios::app);
        file << user.getUserName() << DELIMITER << user.getSalt() << DELIMITER << pass << std::endl;
        file.close();

        std::cout << "User was sucessfully saved." << std::endl;
    }

    void saveUnsafeToFile(User& user, const std::string& pass)
    {
        std::ofstream file;

        file.open(UNSAFE_USERS, std::ios::app);
        file << user.getUserName() << DELIMITER << pass << std::endl;;
        file.close();

        std::cout << "User was sucessfully saved." << std::endl;
    }

    void readAndWriteToFile(const std::string filePath, std::function<void(std::string&, std::ifstream&, std::ofstream&)> doTheThing)
    {
        std::ifstream inFile(filePath);
        std::ofstream outFile("data/tempfile.txt");
        std::string line;

        if(!inFile.is_open() || !outFile.is_open()) 
        {
            std::cerr << "Unable to open the file" << std::endl;
            return;
        }

        doTheThing(line, inFile, outFile);

        inFile.close();
        outFile.close();

        // std::remove(filePath.c_str()); // Delete the original file
        // std::rename("data/temp.txt", filePath.c_str()); // Rename temp file to original file name
    }

    // REMOVE DONT WORK WITH VECTORS STUPID
    std::vector<std::vector<std::string>> fillVectorFromFile(const std::string filePath)
    {
        std::ifstream file(filePath);
        std::vector<std::vector<std::string>> lines;
        std::string line;

        if(!file.is_open()) 
        {
            std::cerr << "Unable to open the file" << std::endl;
            return lines;
        }

        std::cout << "Arrived at vector" << std::endl;
        while(std::getline(file, line))
        {
            std::istringstream iss(line);
            std::string pass, hash;

            if(std::getline(iss, pass, DELIMITER) && std::getline(iss, hash))
            {
                std::vector<std::string> row { pass, hash };
                lines.push_back(row);
            }
        }

        std::cout << "Done with filling vector" << std::endl;
        file.close();
        return lines;
    }

    void sortTextByHash(std::string& line, std::ifstream& inFile, std::ofstream& outFile)
    {
        std::vector<std::vector<std::string>> lines = fillVectorFromFile("data/tempfile.txt");
        std::sort(lines.begin(), lines.end(), [] (std::vector<std::string>& v1, std::vector<std::string>& v2) {
            return v1[1] < v2[1];
        });

        for(int i = 0; i < lines.size(); i++)
        {
            outFile << lines[i][0] << DELIMITER << lines[i][1] << std::endl;
        }
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
                    char randLower = (97 + rand() % 26);
                    if(!containsNumbers(line))
                    {
                        line += "123";
                    }
                    if(!containsLowercase(line))
                    {
                        char randLower = (97 + rand() % 26);
                        line += randLower;
                    }
                    if(!containsSymbols(line))
                    {
                        line += "!";
                    }
                    if(!containsUppercase(line))
                    {
                        if(!containsNumbers(line.substr(0,1)))
                            line[0] = std::toupper(line[0]);
                        else
                        {
                            char randUpper = (65 + rand() % 26);
                            line += randUpper;
                        }
                    }  
                    outFile << line << std::endl;
                }
                else
                {
                    outFile << line << std::endl;
                }
            }
        }
    }

    void appendHashesToExistingPasswords(std::string& line, std::ifstream& inFile, std::ofstream& outFile)
    {
        while(std::getline(inFile, line))
        {
            outFile << line << DELIMITER;
            outFile << Hash::hashPassword(line, false) << std::endl;;
        }
    }

}