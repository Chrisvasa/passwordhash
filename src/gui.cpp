#include <iostream>
#include <string>
#include <chrono>
#include "../include/gui.h"
#include "../include/usermanager.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_stdlib.h"
#include "../include/file.h"

// This class manages the different ImGui components and their input
namespace Application
{
    void LoginWindow(void);
    void PassCrackerWindow(void);
    static std::string _labelPrefix(const char* const label);

    // CRINGE GLOBAL VARIABLE
    char username[50] = {};
    std::string hash = {};
    std::string solved = {};
    std::string input = {};
    std::string output = {};
    char password[50] = {};
    std::string clearpass, t = {};
    bool accountCreated, loginFailed, security, passwordFound;

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
        ImGui::InputText("Enter Hash/Password: ", &hash);
        ImGui::InputText("Input file: ", &input);
        ImGui::InputText("Output file: ", &output);

        if(ImGui::Button("Generate Hashes")) {
            File::readAndWriteToFile(File::appendHashesToExistingPasswords,"data/rockyou.txt", "data/cracked_temp.txt");
            // TAKE TEXTFILE NAME EG: "users.txt" <--- And then add it to a string with data/ 
        }
        if(ImGui::Button("Sort Hashes")) {
            File::readAndWriteToFile(File::sortTextByHash, "data/cracked_temp.txt", "data/crack.txt");
            // TAKE TEXTFILE NAME EG: "users.txt" <--- And then add it to a string with data/ 
        }
        if(ImGui::Button("Find password")) {
            auto startTime = std::chrono::high_resolution_clock::now();
            passwordFound = File::binarySearchInFile(hash, solved, input);
            auto endTime = std::chrono::high_resolution_clock::now();
            std::cout << "\nFinding password took: " << std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count() << " microseconds" << std::endl;
        }
        if(ImGui::Button("Find matches")) {
            std::cout << "Finding matches.." << std::endl;
            auto startTime = std::chrono::high_resolution_clock::now();
            int count = File::findMatches();
            auto endTime = std::chrono::high_resolution_clock::now();
            std::cout << "\nFinding matches took: " << std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count() << " microseconds" << std::endl;
            std::cout << "Found: " << count << std::endl;
        }
        if(passwordFound)
        {
          ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
          ImGui::Text(("The password is: " + solved).c_str());
          ImGui::PopStyleColor();
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
