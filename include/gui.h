#ifndef _GUI_H_
#define _GUI_H_

class Application
{
public:
    Application();
    void RenderUI(void);
private:
    char username[50] = {};
    std::string hash = {};
    std::string solved = {};
    std::string input = {};
    char password[50] = {};
    std::string clearpass, t = {};
    bool accountCreated, loginFailed, security, passwordFound;

    void LoginWindow(void);
    void PassCrackerWindow(void);
    static std::string _labelPrefix(const char* const label);
};

#endif