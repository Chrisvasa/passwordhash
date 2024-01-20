#ifndef _VALIDATE_H_
#define _VALIDATE_H_

#include <functional>
#include <iostream>

bool isValidEmail(const std::string& userName);
bool isValidPassword(const std::string& password);
void getValidInput(std::function<bool(std::string&)> isValid, std::string& input ,const std::string& prompt);

#endif