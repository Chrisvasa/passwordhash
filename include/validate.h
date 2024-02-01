#ifndef _VALIDATE_H_
#define _VALIDATE_H_

#include <functional>
#include <iostream>

bool isValidEmail(const std::string& userName);
bool isValidPassword(const std::string& password);
bool containsNumbers(const std::string& str);
bool containsUppercase(const std::string& str);
bool containsLowercase(const std::string& str);
bool containsSymbols(const std::string& str);

#endif