#pragma once

#define  IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"

#include <string>
#include <vector>
#include <functional>

using namespace std;

#define to_vec4(r, g, b, a) ImColor(r / 255.f, g / 255.f, b / 255.f, a)

class CCustomUI {
public:
    float m_anim = 0.f;

    int m_tab = 0;
    vector<const char*> tabs = { "Local", "AimBot", "Visual", "PlayerList", "System" };
    vector<const char*> tabs_icons = { "L", "A", "V", "P", "S" };

    // Local subtabs
    int m_local_subtab = 0;
    vector<const char*> local_subtabs = { "Player", "Weapon" };

    // Aimbot subtabs
    int m_aimbot_subtab = 0;
    vector<const char*> aimbot_subtabs = { "Main", "Settings", "Keybinds" };

    // Visual subtabs
    int m_visual_subtab = 0;
    vector<const char*> visual_subtabs = { "ESP", "Colors" };

    float col_buf[4] = { 1.f, 1.f, 1.f, 1.f };

    void render_arrows_for_horizontal_bar(ImVec2 pos, float alpha, float width, float height);
    bool expand_button(const char* label, bool selected, float rounding = 0.f, ImDrawFlags flags = NULL);
    bool tab(const char* icon, const char* label, bool selected);
    bool sub_tab(const char* label, bool selected);
    void tab_area(const char* str_id, ImVec2 size, std::function<void()> content);
    void begin_child(const char* name, ImVec2 size);
    void end_child();

    int accent_color[4] = { 255, 128, 0, 255 }; // Orange accent color
    ImColor get_accent_color(float a = 1.f) {
        return to_vec4(accent_color[0], accent_color[1], accent_color[2], a);
    }
};

inline CCustomUI custom;
