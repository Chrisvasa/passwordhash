#include <string>
#include "../include/gui.h"
#include "../include/usermanager.h"
#include "../imgui/imgui.h"


namespace Application
{
    void LoginWindow(void);
    static std::string _labelPrefix(const char* const label);

    // CRINGE GLOBAL VARIABLE
    char username[50] = {}; 
    char password[50] = {};
    bool accountCreated = false;

    void RenderUI(void)
    {
        LoginWindow();
    }

    void LoginWindow(void)
    {
        ImGui::Begin("Account Manager");
        ImGui::InputText(_labelPrefix("Username: ").c_str(), username, sizeof(username));
        ImGui::InputText(_labelPrefix("Password:").c_str(), password, sizeof(password), ImGuiInputTextFlags_Password);

        if(ImGui::Button("Login")) {
            if(authenticateAndLogin(std::string(username), std::string(password))) {
            }
            else {
                memset(password, 0, sizeof(password));
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Create Account"))
        {
            if(createUser(std::string(username), std::string(password)))
            {
                accountCreated = true;
            }
        }

        if(accountCreated) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
            ImGui::Text("Account was sucessfully created!");
            ImGui::PopStyleColor();
            memset(username, 0, sizeof(username));
            memset(password, 0, sizeof(password));
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