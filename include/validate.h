#ifndef _VALIDATE_H_
#define _VALIDATE_H_

#include <functional>
#include <iostream>

bool isValidEmail(std::string userName);
bool isValidPassword(std::string password);
std::string getValidInput(std::function<bool(std::string&)> isValid, std::string prompt);

#endif