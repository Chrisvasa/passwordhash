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
    std::optional<User> getUserFromFile(const std::string& targetUser);
    void saveUserToFile(User& user, const std::string& pass);
    void saveUnsafeToFile(User& user, const std::string& pass);
    void readAndWriteToFile(std::function<void(std::string&, std::ifstream&, std::ofstream&)> doTheThing, const std::string& inPath = "data/users.txt", const std::string& outPath = "data/tempfile.txt");
    std::vector<std::pair<std::string, std::string>> fillPairFromFile(const std::string filePath);
    // Move these to some other file
    void ensureValidPasswords(std::string& line, std::ifstream& inFile, std::ofstream& outFile);
    void appendHashesToExistingPasswords(std::string& line, std::ifstream& inFile, std::ofstream& outFile);
    void sortTextByHash(std::string& line, std::ifstream& inFile, std::ofstream& outFile);
    int findMatches(const std::string& path = "data/tocrack.txt");
}

#endif
