/*
 * TempleWare External Menu System
 * Features:
 * - Local Player Modifications (God Mode, Healing, Weapon Enhancements)
 * - Advanced Aimbot System (Target Selection, FOV Control)
 * - ESP & Visual Enhancements (Box ESP, Skeleton ESP)
 * - Player Management (Player List, Teleport)
 * - System Configuration (Stream Proof, Crosshair)
 */

#include "../core/FrameCore.h"
#include "custom.hpp"

const char* BoxStyleOptions[] = { "Simple", "Cornered" };
const char* TargetingModes[] = { "FOV", "Distance" };
const char* HitboxSelection[] = { "Head", "Chest" };
const char* AimKeyTypeList[] = { "and", "or" };
const char* CrosshairList[] = { "Cross", "Circle" };

void CFramework::DrawModernInterface()
{
    ImGui::SetNextWindowBgAlpha(0.975f);
    ImGui::SetNextWindowSize(ImVec2(725.f, 450.f));
    ImGui::Begin("TempleWare External", &g.MenuVisible, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    auto window = GetCurrentWindow();
    auto draw = window->DrawList;
    auto pos = window->Pos;
    auto size = window->Size;
    auto style = GetStyle();

    custom.m_anim = ImLerp(custom.m_anim, 1.f, 0.03f);
    draw->AddLine(pos + ImVec2(65, 40), pos + ImVec2(size.x - 15, 40), ImColor(1.f, 1.f, 1.f, 0.05f));

    // Tab Area with Expandable Sidebar
    SetCursorPosY(0);
    custom.tab_area("##tab_area", ImVec2(50, size.y - 20), []() {
        for (int i = 0; i < custom.tabs.size(); ++i)
            if (custom.tab(custom.tabs_icons.at(i), custom.tabs.at(i), custom.m_tab == i) && custom.m_tab != i)
                custom.m_tab = i, custom.m_anim = 0.f;
    });

    // Main Content Area
    switch (custom.m_tab) {
    case 0: // Local Player
        SetCursorPos(ImVec2(70, 13));
        BeginGroup();
        for (int i = 0; i < custom.local_subtabs.size(); ++i) {
            if (custom.sub_tab(custom.local_subtabs.at(i), custom.m_local_subtab == i) && custom.m_local_subtab != i)
                custom.m_local_subtab = i, custom.m_anim = 0.f;
            if (i != custom.local_subtabs.size() - 1)
                SameLine();
        }
        EndGroup();

        SetCursorPos(ImVec2(65, 50));
        BeginChild("##local_content", ImVec2(GetWindowWidth() - 80, GetWindowHeight() - 80));

        switch (custom.m_local_subtab) {
        case 0: // Player
            custom.begin_child("Player Settings", ImVec2(GetWindowWidth() / 2 - GetStyle().ItemSpacing.x / 2, GetWindowHeight()));
            Checkbox("GodMode", &g.GodMode);
            Checkbox("Heal Me", &g.AutoHeal);
            custom.end_child();
            break;
        case 1: // Weapon
            custom.begin_child("Weapon Settings", ImVec2(GetWindowWidth() / 2 - GetStyle().ItemSpacing.x / 2, GetWindowHeight()));
            Checkbox("NoRecoil", &g.NoRecoil);
            Checkbox("NoSpread", &g.NoSpread);
            custom.end_child();
            break;
        }
        EndChild();
        break;

    case 1: // AimBot
        SetCursorPos(ImVec2(70, 13));
        BeginGroup();
        for (int i = 0; i < custom.aimbot_subtabs.size(); ++i) {
            if (custom.sub_tab(custom.aimbot_subtabs.at(i), custom.m_aimbot_subtab == i) && custom.m_aimbot_subtab != i)
                custom.m_aimbot_subtab = i, custom.m_anim = 0.f;
            if (i != custom.aimbot_subtabs.size() - 1)
                SameLine();
        }
        EndGroup();

        SetCursorPos(ImVec2(65, 50));
        BeginChild("##aimbot_content", ImVec2(GetWindowWidth() - 80, GetWindowHeight() - 80));

        switch (custom.m_aimbot_subtab) {
        case 0: // Main Settings
            custom.begin_child("Aimbot Settings", ImVec2(GetWindowWidth() / 2 - GetStyle().ItemSpacing.x / 2, GetWindowHeight()));
            Checkbox("AimBot", &g.AimbotEnabled);
            Checkbox("Prediction", &g.Aim_Prediction);
            Checkbox("Aim at NPC", &g.Aim_NPC);
            Checkbox("Draw FOV", &g.Aim_DrawFov);
            Combo("AimBone", &g.Aim_Bone, HitboxSelection, IM_ARRAYSIZE(HitboxSelection));
            custom.end_child();

            SameLine();

            custom.begin_child("FOV Settings", ImVec2(GetWindowWidth() / 2 - GetStyle().ItemSpacing.x / 2, GetWindowHeight()));
            CustomSliderFloat("FOV", "##aim_fov", &g.Aim_Fov, 50.f, 1000.f);
            ColorEdit4("FOV Color", &FOV_User.Value.x);
            custom.end_child();
            break;

        case 1: // Advanced Settings
            custom.begin_child("Advanced Settings", ImVec2(GetWindowWidth() / 2 - GetStyle().ItemSpacing.x / 2, GetWindowHeight()));
            CustomSliderFloat("Smooth", "##aim_smt", &g.Aim_Smooth, 1.f, 20.f);
            CustomSliderFloat("Distance", "##aim_dist", &g.Aim_MaxDistance, 50.f, 300.f);
            CustomSliderFloat("Predict", "##aim_predict", &g.Aim_Predict, 1000.f, 3000.f);
            Combo("AimType", &g.Aim_Type, TargetingModes, IM_ARRAYSIZE(TargetingModes));
            custom.end_child();
            break;

        case 2: // Keybinds
            custom.begin_child("Key Bindings", ImVec2(GetWindowWidth() / 2 - GetStyle().ItemSpacing.x / 2, GetWindowHeight()));
            Text("Primary Key:");
            if (Button(BindingID == 1 ? "< Press Any Key >" : KeyNames[g.AimKey0], ImVec2(215.f, 22.5f))) {
                BindingID = 1;
                std::thread([&]() {KeyBinder(g.AimKey0, BindingID); }).detach();
            }

            Text("Secondary Key:");
            if (Button(BindingID == 2 ? "< Press Any Key >" : KeyNames[g.AimKey1], ImVec2(215.f, 22.5f))) {
                BindingID = 2;
                std::thread([&]() {KeyBinder(g.AimKey1, BindingID); }).detach();
            }
            custom.end_child();
            break;
        }
        EndChild();
        break;

    case 2: // Visual
        SetCursorPos(ImVec2(70, 13));
        BeginGroup();
        for (int i = 0; i < custom.visual_subtabs.size(); ++i) {
            if (custom.sub_tab(custom.visual_subtabs.at(i), custom.m_visual_subtab == i) && custom.m_visual_subtab != i)
                custom.m_visual_subtab = i, custom.m_anim = 0.f;
            if (i != custom.visual_subtabs.size() - 1)
                SameLine();
        }
        EndGroup();

        SetCursorPos(ImVec2(65, 50));
        BeginChild("##visual_content", ImVec2(GetWindowWidth() - 80, GetWindowHeight() - 80));

        switch (custom.m_visual_subtab) {
        case 0: // ESP Settings
            custom.begin_child("ESP Options", ImVec2(GetWindowWidth() / 2 - GetStyle().ItemSpacing.x / 2, GetWindowHeight()));
            Checkbox("ESP", &g.WallhackEnabled);
            Checkbox("NPC ESP", &g.ESP_NPC);
            Checkbox("Box", &g.ESP_Box);
            Checkbox("Box Filled", &g.ESP_BoxFilled);
            Checkbox("Line", &g.ESP_Line);
            Checkbox("Name", &g.ESP_Name);
            Checkbox("Skeleton", &g.ESP_Skeleton);
            Checkbox("Distance", &g.ESP_Distance);
            Checkbox("Health Bar", &g.ESP_HealthBar);
            custom.end_child();

            SameLine();

            custom.begin_child("ESP Settings", ImVec2(GetWindowWidth() / 2 - GetStyle().ItemSpacing.x / 2, GetWindowHeight()));
            CustomSliderFloat("Distance", "##Dist", &g.ESP_MaxDistance, 100.f, 2000.f);
            Combo("Box Type", &g.ESP_BoxType, BoxStyleOptions, IM_ARRAYSIZE(BoxStyleOptions));
            custom.end_child();
            break;

        case 1: // Colors
            custom.begin_child("ESP Colors", ImVec2(GetWindowWidth() / 2 - GetStyle().ItemSpacing.x / 2, GetWindowHeight()));
            ColorEdit4("Box Fill", &ESP_Filled.Value.x);
            custom.end_child();
            break;
        }
        EndChild();
        break;

    case 3: // PlayerList
        SetCursorPos(ImVec2(65, 50));
        BeginChild("##playerlist_content", ImVec2(GetWindowWidth() - 80, GetWindowHeight() - 80));

        custom.begin_child("Player List", ImVec2(GetWindowWidth(), GetWindowHeight()));
        static char TargetName[64];
        InputText("Search Player", TargetName, IM_ARRAYSIZE(TargetName));

        if (BeginTable("PlayerListTable", 5, ImGuiTableFlags_Borders)) {
            TableNextColumn(); Text("Name");
            TableNextColumn(); Text("Alive");
            TableNextColumn(); Text("Vehicle");
            TableNextColumn(); Text("Position");
            TableNextColumn(); Text("Teleport");

            for (auto& ped : EntityList) {
                if (!ped.IsPlayer())
                    continue;

                char pN[32]{};
                m.ReadString(ped.PlayerInfo + 0xFC, pN, sizeof(pN));
                std::string pName = pN;

                if (TargetName != "\0" && pName.find(TargetName) == std::string::npos)
                    continue;

                TableNextColumn(); Text(pName.c_str());
                TableNextColumn(); TextColored(!ped.IsDead() ? ImColor(0.f, 1.f, 0.f, 1.f) : ImColor(1.f, 0.f, 0.f, 1.f), !ped.IsDead() ? "Yes" : "No");
                TableNextColumn(); TextColored(ped.InVehicle() ? ImColor(0.f, 1.f, 0.f, 1.f) : ImColor(1.f, 0.f, 0.f, 1.f), ped.InVehicle() ? "Yes" : "No");
                TableNextColumn(); Text("%.f, %.f, %.f", ped.m_pVecLocation.x, ped.m_pVecLocation.y, ped.m_pVecLocation.z);
                TableNextColumn();
                std::string button_string = "Teleport##" + pName;
            }
            EndTable();
        }
        custom.end_child();
        EndChild();
        break;

    case 4: // System
        SetCursorPos(ImVec2(65, 50));
        BeginChild("##system_content", ImVec2(GetWindowWidth() - 80, GetWindowHeight() - 80));

        custom.begin_child("System Settings", ImVec2(GetWindowWidth() / 2 - GetStyle().ItemSpacing.x / 2, GetWindowHeight()));
        Checkbox("Stream Proof", &g.StreamProof);
        custom.end_child();

        SameLine();

        custom.begin_child("Crosshair Settings", ImVec2(GetWindowWidth() / 2 - GetStyle().ItemSpacing.x / 2, GetWindowHeight()));
        Checkbox("Crosshair", &g.Crosshair);
        CustomSliderInt("Size", "##SizeCH", &g.CrosshairSize, 1, 10);
        ColorEdit4("Color##C", &CrosshairColor.Value.x);
        Combo("Type##C", &g.CrosshairType, CrosshairList, IM_ARRAYSIZE(CrosshairList));

        Spacing();
        NewLine();

        if (Button("Exit", ImVec2(GetContentRegionAvail().x, 30.f)))
            g.Run = false;
        custom.end_child();

        EndChild();
        break;
    }

    // Footer
    draw->AddRectFilled(pos + ImVec2(0, size.y - 20), pos + size, ImColor(15, 14, 21), style.WindowRounding, ImDrawFlags_RoundCornersBottom);
    draw->AddText(pos + ImVec2(5, size.y - 18), GetColorU32(ImGuiCol_Text), "TempleWare External for FiveM");

    ImGui::End();
}
