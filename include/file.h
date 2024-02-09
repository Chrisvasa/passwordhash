#ifndef _FILE_H_
#define _FILE_H_
#include <fstream>
#include <string>
#include <functional>
#include <optional>
#include "user.h"

namespace File
{
    bool binarySearchInFile(const std::string& targetVal, const std::string& path = "data/crack.txt");
    bool binarySearchInFile(const std::string& targetVal, std::string& foundVal, const std::string& path = "data/crack.txt");
    std::optional<User> getUserFromFile(const std::string& targetUser, const std::string& pass);
    void saveUserToFile(const std::string& username, const std::string& salt, const std::string& pass);
    void saveUnsafeToFile(const std::string& username, const std::string& pass);
    void readAndWriteToFile(std::function<void(std::string&, std::ifstream&, std::ofstream&)> doTheThing, const std::string& inPath = "data/users.txt", const std::string& outPath = "data/tempfile.txt");
    void ensureValidPasswords(std::string& line, std::ifstream& inFile, std::ofstream& outFile);
    void appendHashesToExistingPasswords(std::string& line, std::ifstream& inFile, std::ofstream& outFile);
    int findMatches(const std::string& path = "data/tocrack.txt");
}

#endif
