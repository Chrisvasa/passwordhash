#include <iostream>
#include <string>
#include <chrono>
#include "../include/gui.h"
#include "../include/usermanager.h"
#include "../imgui/imgui.h"
#include "../include/file.h"

// This class manages the different ImGui components and their input
namespace Application
{
    void LoginWindow(void);
    void PassCrackerWindow(void);
    static std::string _labelPrefix(const char* const label);

    // CRINGE GLOBAL VARIABLE
    char username[50] = {};
    char hash[50] = {}; 
    char password[50] = {};
    std::string clearpass = {};
    bool accountCreated = false;
    bool loginFailed = false;
    bool security = false;

    void RenderUI(void)
    {
        LoginWindow();
        PassCrackerWindow();
    }

    void LoginWindow(void)
    {
        ImGui::Begin("Account Manager");
        ImGui::InputText(_labelPrefix("Username: ").c_str(), username, sizeof(username));
        ImGui::InputText(_labelPrefix("Password:").c_str(), password, sizeof(password), ImGuiInputTextFlags_Password);

        if(ImGui::Button("Login")) {
            if(authenticateAndLogin(std::string(username), std::string(password))) {
                std::cout << "Login success!" << std::endl;
                loginFailed = false;
            }
            else {
                loginFailed = true;
                memset(password, 0, sizeof(password)); // Maybe not needed? Check if string possible
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Create Account"))
        {
            if(createUser(std::string(username), std::string(password), security)) {
                accountCreated = true;
                memset(username, 0, sizeof(username)); // Maybe not needed? Check if string possible
                memset(password, 0, sizeof(password)); // Maybe not needed? Check if string possible
            }
        }
        ImGui::SameLine();
        ImGui::Checkbox("Extra security?", &security);


        if(loginFailed)
        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
            ImGui::Text("Login failed.");
            ImGui::PopStyleColor();
        }
        if(accountCreated) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
            ImGui::Text("Account was sucessfully created!");
            ImGui::PopStyleColor();
        }

        ImGui::End();
    }

    void PassCrackerWindow(void)
    {
        ImGui::Begin("Password Cracker");
        ImGui::InputText(_labelPrefix("Enter Hash: ").c_str(), hash, sizeof(hash));
        if(ImGui::Button("Generate Hashes")) {
            File::readAndWriteToFile("data/rockyou.txt", "data/cracked_temp.txt", File::appendHashesToExistingPasswords);
            // TAKE TEXTFILE NAME EG: "users.txt" <--- And then add it to a string with data/ 
        }
        if(ImGui::Button("Sort Hashes")) {
            File::readAndWriteToFile("data/cracked.txt", "data/crack.txt", File::sortTextByHash);
            // TAKE TEXTFILE NAME EG: "users.txt" <--- And then add it to a string with data/ 
        }
        if(ImGui::Button("Find password")) {
            auto startTime = std::chrono::high_resolution_clock::now();
            File::binarySearchInFile("data/cracked.txt", hash);
            auto endTime = std::chrono::high_resolution_clock::now();
            std::cout << "\nFinding password took: " << std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count() << " microseconds" << std::endl;
        }
        if(ImGui::Button("Find matches")) {
            std::cout << "Finding matches.." << std::endl;
            auto startTime = std::chrono::high_resolution_clock::now();
            File::findMatches();
            auto endTime = std::chrono::high_resolution_clock::now();
            std::cout << "\nFinding matches took: " << std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count() << " microseconds" << std::endl;
        }
        ImGui::End();
    }

    static std::string _labelPrefix(const char* const label)
    {
        float width = ImGui::CalcItemWidth();

        float x = ImGui::GetCursorPosX();
        ImGui::Text(label); 
        ImGui::SameLine(); 
        ImGui::SetCursorPosX(x + width * 0.5f + ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::SetNextItemWidth(-1);

        std::string labelID = "##";
        labelID += label;

        return labelID;
    }
}