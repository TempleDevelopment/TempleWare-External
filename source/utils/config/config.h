#pragma once
#include <Windows.h>

struct Globals
{

    bool Run = true;
    bool MenuVisible = false;

    HWND GameHwnd;
    RECT GameRect{};
    POINT GamePoint{};

    bool AimbotEnabled = true;
    bool Aim_NPC = false;
    bool Aim_Prediction = true;
    bool Aim_DrawFov = true;

    float Aim_Fov = 150.f;
    float Aim_Smooth = 1.f;
    float Aim_MaxDistance = 150.f;
    float Aim_Predict = 2000.f;

    int Aim_Bone = 0;
    int Aim_Type = 0;
    int AimKeyType = 1;

    bool WallhackEnabled = true;
    bool ESP_NPC = false;
    bool ESP_Box = true;
    bool ESP_BoxFilled = false;
    bool ESP_Line = false;
    bool ESP_Name = true;
    bool ESP_Skeleton = true;
    bool ESP_Distance = true;
    bool ESP_HealthBar = true;
    int ESP_BoxType = 1;
    float ESP_MaxDistance = 500.f;

    bool GodMode = false;
    bool AutoHeal = false;
    bool NoRecoil = false;
    bool NoSpread = false;

    bool StreamProof = false;
    bool Crosshair = false;
    int CrosshairType = 0;
    int CrosshairSize = 3;

    int MenuKey = VK_END;
    int AimKey0 = VK_RBUTTON;
    int AimKey1 = VK_LBUTTON;
};

extern Globals g;
