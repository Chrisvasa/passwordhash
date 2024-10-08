#ifndef _VALIDATE_H_
#define _VALIDATE_H_

#include <iostream>

bool isValidEmail(const std::string& userName);
bool isValidPassword(const std::string& password);
bool equalsIgnoreCase(const std::string& a, const std::string& b);
bool containsDigit(const std::string& str);
bool containsUppercase(const std::string& str);
bool containsLowercase(const std::string& str);
bool containsSymbols(const std::string& str);

#endif
