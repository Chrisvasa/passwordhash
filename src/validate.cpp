#include <regex>

bool isValidEmail(const std::string& userName)
{
    std::regex pattern("^[A-Za-z]{2,}@[A-Za-z]+\\.[A-Za-z]{2,}"); // Ex: gg@gg.gg
    return (userName.length() > 7 && std::regex_match(userName, pattern));
}

bool isValidPassword(const std::string& password)
{
    std::regex pattern("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[^a-zA-Z\\d]).+$"); // Ex: Ggg1234!
    return (password.length() > 7 && std::regex_match(password, pattern));
}

// USED FOR PASSWORD GENERATION
bool containsNumbers(const std::string& str) 
{
    return std::regex_search(str, std::regex("\\d"));
}

bool containsUppercase(const std::string& str) 
{
    return std::regex_search(str, std::regex("[A-Z]"));
}

bool containsLowercase(const std::string& str) 
{
    return std::regex_search(str, std::regex("[a-z]"));
}

bool containsSymbols(const std::string& str) 
{
    return std::regex_search(str, std::regex("[^\\w\\s]")); // or use "[\\.,\\?!]" for specific symbols
}
