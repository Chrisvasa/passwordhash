#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <optional>
#include <cctype>
#include <functional>
#include <openssl/evp.h>
#include "../include/file.h"
#include "../include/validate.h"
#include "../include/hash.h"

// Manages all file access functions
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

    void saveUserToFile(User& user, const std::string& pass)
    {
        std::string filename = "data/users.txt";
        std::ofstream file;

        file.open(filename, std::ios::app);
        if(!isEmptyFile(filename))
            file << std::endl;
        file << user.getUserName() << ";" << user.getSalt() << ";" << pass;
        file.close();

        std::cout << "User was sucessfully saved." << std::endl;
    }

    void saveUnsafeToFile(User& user, const std::string& pass)
    {
        std::string filename = "data/unsafe_users.txt";
        std::ofstream file;

        file.open(filename, std::ios::app);
        if(!isEmptyFile(filename))
            file << std::endl;
        file << user.getUserName() << ";" << pass;
        file.close();

        std::cout << "User was sucessfully saved." << std::endl;
    }

    void readAndWriteToFile(const std::string filePath, std::function<void(std::string&, std::ifstream&, std::ofstream&)> doTheThing)
    {
        std::ifstream inFile(filePath);
        std::ofstream outFile("data/temp.txt");
        std::string line;

        if(!inFile.is_open() || !outFile.is_open()) 
        {
            std::cerr << "Unable to open the file" << std::endl;
            return;
        }

        doTheThing(line, inFile, outFile);

        inFile.close();
        outFile.close();

        std::remove(filePath.c_str()); // Delete the original file
        std::rename("data/temp.txt", filePath.c_str()); // Rename temp file to original file name
    }

    void sortByHash(std::string& line, std::ifstream& inFile, std::ofstream& outFile)
    {
        std::vector<std::vector<std::string>> lines;
        while(std::getline(inFile, line))
        {
            std::istringstream iss(line);
            std::string pass, hash;

            if(std::getline(iss, pass, ';') && std::getline(iss, hash))
            {
                std::vector<std::string> row { pass, hash };
                lines.push_back(row);
            }
        }

        std::sort(lines.begin(), lines.end(), [] (std::vector<std::string>& v1, std::vector<std::string>& v2) {
            return v1[1] < v2[1];
        });

        for(int i = 0; i < lines.size(); i++)
        {
            outFile << lines[i][0] << ";" << lines[i][1] << std::endl;
        }
    }

    void passwordValidator(std::string& line, std::ifstream& inFile, std::ofstream& outFile)
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

    void passwordHasher(std::string& line, std::ifstream& inFile, std::ofstream& outFile)
    {
        while(std::getline(inFile, line))
        {
            outFile << line << ";";
            outFile << Hash::hashPassword(line, false) << std::endl;;
        }
    }

}


/*Läsa in från fil
Varje lösenord blir 3? nya i en annan fil
Exempel:
    hejsan  -> Hejsan123!
            -> Hejsan321!
            -> Hejsan2024!
    aaaaa   -> Aaaaa123!
            -> Aaaaa321!
            -> aAAAA123!

*/