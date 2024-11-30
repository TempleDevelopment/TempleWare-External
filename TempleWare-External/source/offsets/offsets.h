#pragma once
#include <cstdint>

namespace offsets {
    inline constexpr auto build_number = 14057;
    inline constexpr auto dwBuildNumber = 0x531BE4;
    inline constexpr auto dwLocalPlayerController = 0x1A41FD0;
    inline constexpr auto dwLocalPlayer = 0x1A0C310;  // Local player pawn offset
    inline constexpr auto dwEntityList = 0x19F2488;
    inline constexpr auto dwViewMatrix = 0x1A54550;
    inline constexpr auto dwPlantedC4 = 0x1A59ED0;
    inline constexpr auto m_iHealth = 0x324;
    inline constexpr auto m_iTeamNum = 0x3C3;
    inline constexpr auto m_vOldOrigin = 0x1274;
    inline constexpr auto m_hPlayerPawn = 0x80C;

    // Additional offsets required by the codebase
    inline constexpr auto m_Glow = 0xBA0;
    inline constexpr auto m_glowColorOverride = 0x1B78;
    inline constexpr auto m_bGlowing = 0x1B80;
    inline constexpr auto m_flFlashDuration = 0x1468;
    inline constexpr auto m_iIDEntIndex = 0x1544;
    inline constexpr auto attack = 0x1738B60;
    inline constexpr auto flFlashDuration = 0x1468;
    inline constexpr auto m_pCameraServices = 0x11E0;
    inline constexpr auto m_iFOV = 0x210;
    inline constexpr auto m_bIsScoped = 0x23E8;
    inline constexpr auto m_fFlags = 0x3EC;
}
