#include "gui.h"
#include "imgui.h"

namespace Application
{
    void RenderUI(void)
    {
        ImGui::Begin("Hello");
        ImGui::Button("Hello");
        ImGui::End();
    }
}