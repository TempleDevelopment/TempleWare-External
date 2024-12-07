#include "imgui.h"
#include "imgui_internal.h"

namespace ImGui
{
    // system
    void RenderCustomFrame(ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, ImU32 sub_col, bool border, float rounding);

    // Button
    bool CustomButton(const char* label, const ImVec2& size_arg, const bool& flag);
}