#include <iostream>
#include <string>
#include <chrono>
#include <cstdlib>
#include "../include/gui.h"
#include "../include/usermanager.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_stdlib.h"
#include "../imgui/ImGuiFileDialog.h"
#include "../include/file.h"

// Manages all the different GUI aspects
// Draws the GUI and handles the different actions

Application::Application() {}

void Application::RenderUI(void)
{
  LoginWindow();
  PassCrackerWindow();
  FileManagerWindow();
}

void Application::LoginWindow(void)
{
    ImGui::Begin("Account Manager");
    ImGui::InputText(_labelPrefix("Username: ").c_str(), &username);
    ImGui::InputText(_labelPrefix("Password:").c_str(), &password, ImGuiInputTextFlags_Password);

    if(ImGui::Button("Login")) {
        if(Manage::authenticateAndLogin(std::string(username), std::string(password))) 
        {
            std::cout << "Login success!" << std::endl;
            username = {};
            password = {};
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
        if(Manage::createUser(std::string(username), std::string(password), security)) 
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
    ImGui::InputText(_labelPrefix("Enter Hash:").c_str(), &hash);

    if(ImGui::Button("Crack Hash")) 
    {
        auto startTime = std::chrono::high_resolution_clock::now();
        if(searchFile.empty()) 
          passwordFound = File::binarySearchInFile(hash, solved);
        else 
          passwordFound = File::binarySearchInFile(hash, solved, searchFile);
        auto endTime = std::chrono::high_resolution_clock::now();
        std::cout << "\nFinding password took: " << std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count() << " microseconds" << std::endl;
    }
    ImGui::SameLine();
    ImGui::Text("Attempts to find matching password to given hash");

    if(ImGui::Button("Open File"))
    {
      IGFD::FileDialogConfig config;
      config.path = "data/";
      ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".txt", config);
    }
    ImGui::SameLine();
    ImGui::Text("Select a file that you wish to crack.");
    ImGui::Text("Currently selected file: %s",  searchFile.c_str());

    if(ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
    {
      if(ImGuiFileDialog::Instance()->IsOk())
        searchFile = ImGuiFileDialog::Instance()->GetFilePathName();
      ImGuiFileDialog::Instance()->Close();
    }

    if(ImGui::Button("Find matches")) 
    {
        std::cout << "Finding matches.." << std::endl;
        int count = 0;
        auto startTime = std::chrono::high_resolution_clock::now();
        if(searchFile.empty())
          count = File::findMatches();
        else
          count = File::findMatches(searchFile);
        auto endTime = std::chrono::high_resolution_clock::now();
        std::cout << "\nFinding matches took: " << std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count() << " microseconds" << std::endl;
        std::cout << "Found: " << count << std::endl;
    }
    ImGui::SameLine();
    ImGui::Text("Attempts to find as many matches as possible to your Hash file.");

    if(passwordFound)
    {
      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f)); // Changes text to green
      ImGui::Text("%s", ("The password is: " + solved).c_str());
      ImGui::PopStyleColor();
    }
    ImGui::End();
}

void Application::FileManagerWindow()
{
    if(ImGui::Button("Open File"))
    {
      IGFD::FileDialogConfig config;
      config.path = "data/";
      ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey1", "Choose File", ".txt", config);
    }
    ImGui::SameLine();
    ImGui::Text("Select a file that you modify");
    ImGui::Text("Currently selected file: %s",  inputFile.c_str());

    if(ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey1"))
    {
      if(ImGuiFileDialog::Instance()->IsOk())
        inputFile = ImGuiFileDialog::Instance()->GetFilePathName();
      ImGuiFileDialog::Instance()->Close();
    }

    if(ImGui::Button("Sort file by Hash"))
    {
      std::string command = "sort -t ';' -k2,2 " + inputFile + " -o " + inputFile;
      int result = system(command.c_str());
      if (result != 0) {
          // Handle the error
      }
    }

    if(ImGui::Button("Generate Hashes")) 
    {
      File::readAndWriteToFile(File::appendHashesToExistingPasswords, inputFile);
    }
    ImGui::SameLine();
    ImGui::Text("Appends Hashes to file with passwords.");

  if(ImGui::Button("Make valid password"))
  {
      File::readAndWriteToFile(File::ensureValidPasswords, inputFile);
    }
    ImGui::SameLine();
    ImGui::Text("Takes a file of passwords and makes them valid for our program.");
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
