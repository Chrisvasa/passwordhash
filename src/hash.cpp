#include <openssl/evp.h>
#include <iomanip>
#include <random>
#include "../include/hash.h"

/*  Handles the security aspect of the program
    Hashes, salts and checks validity of existing passwords
*/

namespace Hash
{
    std::string hashPassword(const std::string& password, bool security)
    {
        EVP_MD_CTX* mdctx; // Pointer to the digest context
        const EVP_MD* md; // Pointer to the digest method (MD5 in this case)
        unsigned char md_value[EVP_MAX_MD_SIZE]; // Array to store the hash result
        unsigned int md_len, i;

        if(security)
            md = EVP_sha256(); // selects the sha256 algorithm.
        else
            md = EVP_md5(); // selects the MD5 algorithm.
        mdctx = EVP_MD_CTX_new(); //creates a context for the digest.
        EVP_DigestInit_ex(mdctx, md, NULL); // initializes the digest context.
        EVP_DigestUpdate(mdctx, password.c_str(), password.size()); // hashes the data.
        EVP_DigestFinal_ex(mdctx, md_value, &md_len); // retrieves the final digest.
        EVP_MD_CTX_free(mdctx); // cleans up the context.

        std::stringstream hash;
        for(i = 0; i < md_len; i++)
            hash << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(md_value[i]);

        return hash.str();
    }

    bool isPasswordSecure(const std::string& pass)
    {
        return pass.length() > 16 ? true: false;
    }

    bool validatePassword(const std::string& pass, const std::string& salt, const std::string password)
    {
        bool secure = isPasswordSecure(password);
        std::string pw = hashPassword(salt + pass, secure);
        if(pw == password)
            return true;
        return false;
    }

    std::string generateSalt()
    {
        const std::string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        std::string salt = {};
        std::minstd_rand randomNum(std::random_device{}());
        std::uniform_int_distribution<> dist(0, chars.size() - 1);

        for (int i = 0; i < 16; i++)
            salt += chars[dist(randomNum)];

        return salt;
    }
}
