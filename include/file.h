#ifndef _FILE_H_
#define _FILE_H_
#include <string>
#include <functional>
#include <optional>
#include "user.h"

namespace File
{
    bool binarySearchInFile(const std::string& path, const std::string& targetVal);
    bool binarySearchInFile(const std::string& path, const std::string& targetVal, std::string& foundVal);
    std::optional<User> getUserFromFile(const std::string& targetUser);
    void saveUserToFile(User& user, const std::string& pass);
    void saveUnsafeToFile(User& user, const std::string& pass);
    void readAndWriteToFile(const std::string filePath, const std::string oFilePath, std::function<void(std::string&, std::ifstream&, std::ofstream&)> doTheThing);
    std::vector<std::pair<std::string, std::string>> fillPairFromFile(const std::string filePath);
    // Move these to some other file
    void ensureValidPasswords(std::string& line, std::ifstream& inFile, std::ofstream& outFile);
    void appendHashesToExistingPasswords(std::string& line, std::ifstream& inFile, std::ofstream& outFile);
    void sortTextByHash(std::string& line, std::ifstream& inFile, std::ofstream& outFile);
    int findMatches();
}

#endif
