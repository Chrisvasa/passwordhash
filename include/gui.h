#ifndef _GUI_H_
#define _GUI_H_
#include <iostream>

class Application
{
public:
    Application();
    void RenderUI(void);
private:
    std::string username = {};
    std::string hash = {};
    std::string solved = {};
    std::string searchFile = {};
    std::string inputFile = {};
    std::string password = {};
    bool accountCreated = false;
    bool loginFailed = false;
    bool security = false;
    bool passwordFound = false;

    void LoginWindow(void);
    void PassCrackerWindow(void);
    void FileManagerWindow(void);
    std::string _labelPrefix(const char* const label);
    void toggleBool(bool& b);
};

#endif
