#ifndef _FILE_H_
#define _FILE_H_
#include <string>
#include <functional>
#include <optional>
#include "user.h"

namespace File
{
    std::optional<User> getUserFromFile(const std::string& targetUser);
    void saveUserToFile(User& user, const std::string& pass);
    void saveUnsafeToFile(User& user, const std::string& pass);
    void readAndWriteToFile(const std::string filePath, std::function<void(std::string&, std::ifstream&, std::ofstream&, const std::string hash)> doTheThing, const std::string hash);
    void fillVector(std::string& line, std::ifstream& inFile, std::ofstream& outFile, const std::string hash);
    void binaryHash(std::vector<std::vector<std::string>>& line, const std::string hash);
    void passwordValidator(std::string& line, std::ifstream& inFile, std::ofstream& outFile);
    void passwordHasher(std::string& line, std::ifstream& inFile, std::ofstream& outFile);
    // void sortByHash(std::string& line, std::ifstream& inFile, std::ofstream& outFile);
}

#endif