#include <iostream>
#include <string>
#include <chrono>
#include "../include/gui.h"
#include "../include/usermanager.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_stdlib.h"
#include "../include/file.h"

Application::Application() {}

// This class manages the different ImGui components and their input
void Application::RenderUI(void)
{
  LoginWindow();
  PassCrackerWindow();
}

void Application::LoginWindow(void)
{
    ImGui::Begin("Account Manager");
    ImGui::InputText(_labelPrefix("Username: ").c_str(), &username);
    ImGui::InputText(_labelPrefix("Password:").c_str(), &password, ImGuiInputTextFlags_Password);

    if(ImGui::Button("Login")) {
        if(authenticateAndLogin(std::string(username), std::string(password))) 
        {
            std::cout << "Login success!" << std::endl;
            loginFailed = false;
        }
        else 
        {
            toggleBool(loginFailed);
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Create Account"))
    {
        if(createUser(std::string(username), std::string(password), security)) 
        {
          toggleBool(accountCreated); 
        }
    }

    ImGui::SameLine();
    ImGui::Checkbox("Extra security?", &security);
    if(loginFailed)
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); // Changes text to red
        ImGui::Text("Login failed.");
        ImGui::PopStyleColor();
    }
    if(accountCreated) 
    {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f)); // Changes text to green
        ImGui::Text("Account was sucessfully created!");
        ImGui::PopStyleColor();
    }
    ImGui::End();
}

void Application::PassCrackerWindow(void)
{
    ImGui::Begin("Password Cracker");
    ImGui::InputText(_labelPrefix("Passowrd/Hash").c_str(), &hash);
    ImGui::InputText(_labelPrefix("Input filepath:").c_str(), &input);

    if(ImGui::Button("Generate Hashes")) 
    {
      // SHOULD ONLY BE USED WITH FRESH PASSWORD FILE - Will append hash to whatever text is infront of it.
      File::readAndWriteToFile(File::appendHashesToExistingPasswords, input);
    }
    if(ImGui::Button("Sort Hashes")) 
    {
      if(input.empty()) // File for sorting hashes - STANDARD IS users.txt 
        File::readAndWriteToFile(File::sortTextByHash);
      else
        File::readAndWriteToFile(File::sortTextByHash, input);
    }
    if(ImGui::Button("Find password")) 
    {
        auto startTime = std::chrono::high_resolution_clock::now();
        if(input.empty()) {// File for binarySearch - STANDARD IS crack.txt
          passwordFound = File::binarySearchInFile(hash, solved);
          toggleBool(passwordFound);
        }
        else {
          passwordFound = File::binarySearchInFile(hash, solved, input);
          toggleBool(passwordFound);
        }
        auto endTime = std::chrono::high_resolution_clock::now();
        std::cout << "\nFinding password took: " << std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count() << " microseconds" << std::endl;
    }
    if(ImGui::Button("Find matches")) 
    {
        std::cout << "Finding matches.." << std::endl;
        int count = 0;
        auto startTime = std::chrono::high_resolution_clock::now();
        if(input.empty()) // File for passwords to crack - STANDARD IS tocrack.txt 
          count = File::findMatches();
        else
          count = File::findMatches(input);
        auto endTime = std::chrono::high_resolution_clock::now();
        std::cout << "\nFinding matches took: " << std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count() << " microseconds" << std::endl;
        std::cout << "Found: " << count << std::endl;
    }
    if(ImGui::Button("Make valid password")) {
      File::readAndWriteToFile(File::ensureValidPasswords, input);
    }

    if(passwordFound)
    {
      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f)); // Changes text to green
      ImGui::Text("%s", ("The password is: " + solved).c_str());
      ImGui::PopStyleColor();
    }
    ImGui::End();
}

std::string Application::_labelPrefix(const char* const label)
{
    float width = ImGui::CalcItemWidth();
    float x = ImGui::GetCursorPosX();
    ImGui::Text("%s", label); 
    ImGui::SameLine(); 
    ImGui::SetCursorPosX(x + width * 0.5f + ImGui::GetStyle().ItemInnerSpacing.x);
    ImGui::SetNextItemWidth(-1);
    std::string labelID = "##";
    labelID += label;
    return labelID;
}

void Application::toggleBool(bool& b)
{
  accountCreated = false;
  loginFailed = false;
  security = false;
  passwordFound = false;

  b = true;
}
