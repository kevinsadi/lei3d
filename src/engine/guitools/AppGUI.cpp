#include "AppGUI.hpp"

namespace lei3d {
	void AppGUI::RenderUI() {
        if (!ImGui::Begin("Dear ImGui Demo", p_open, window_flags))
        {
            // Early out if the window is collapsed, as an optimization.
            ImGui::End();
            return;
        }

        // Menu Bar
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Menu")) 
            {
                ImGui::MenuItem("Show Demo Window", NULL, &m_ShowDemoWindow);
            }
        }
#ifdef SHOW_DEMO_WINDOW
        ImGui::ShowDemoWindow();
#else   //#ifdef SHOW_DEMO_WINDOW
        ImGui::Begin("LEI 3D Menu");
        ImGui::Text("PRESS TAB TO UNLOCK THE MOUSE");
        ImGui::Text("fps = %f", 1.0f / Application::Curr()->DeltaTime());
        ImGui::SetWindowSize(ImVec2(0, 0));
        ImGui::End();

        //ImGui::Begin("Scene Window");
        //ImGui::Text("Yayaya");
        //ImGui::End();
#endif
	}


}