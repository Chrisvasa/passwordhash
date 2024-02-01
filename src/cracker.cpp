#include <iostream>
#include <vector>
#include "../include/file.h"

std::string findPasswordByHash(const std::string hash)
{
    std::vector<std::vector<std::string>> lines = File::fillVectorFromFile("data/passhash.txt");
    int left = 0;
    int right = lines.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (lines[mid][1] == hash) {
            std::cout << "Found it: " << lines[mid][1] << " And their password is: " << lines[mid][0] << std::endl;
            return lines[mid][0];
        } 
        else if (lines[mid][1] < hash) {
            left = mid + 1;
        } 
        else {
            right = mid - 1;
        }
    }
    std::cout << "Hash not found." << std::endl;
    return "";
}