#include <iostream>
#include <string>
#include "../include/gui.h"
#include "../include/usermanager.h"
#include "../imgui/imgui.h"

// This class manages the different ImGui components and their input
namespace Application
{
    void LoginWindow(void);
    void PassCrackerWindow(void);
    static std::string _labelPrefix(const char* const label);

    // CRINGE GLOBAL VARIABLE
    char username[50] = {}; 
    char password[50] = {};
    bool accountCreated = false;
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
            }
            else {
                memset(password, 0, sizeof(password));
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Create Account"))
        {
            // Add bool check later - Button to press? MD5 or SHA256
            if(createUser(std::string(username), std::string(password), security)) {
                accountCreated = true;
            }
        }
        ImGui::SameLine();
        ImGui::Checkbox("Extra security?", &security);

        if(accountCreated) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
            ImGui::Text("Account was sucessfully created!");
            ImGui::PopStyleColor();
            memset(username, 0, sizeof(username));
            memset(password, 0, sizeof(password));
        }

        ImGui::End();
    }

    void PassCrackerWindow(void)
    {
        ImGui::Begin("Password Cracker");
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