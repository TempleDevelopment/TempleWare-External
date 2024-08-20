#pragma once

#include <cstddef>
#include <locale>

#include "../../imgui/imgui.h"

namespace offsets {
    // Game offsets
    constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x17D4810;
    constexpr std::ptrdiff_t dwEntityList = 0x1969518;
    constexpr std::ptrdiff_t dwViewMatrix = 0x19CB4A0;
    constexpr std::ptrdiff_t dwViewAngles = 0x19DA8F8;

    // Player details
    constexpr std::ptrdiff_t m_pCameraServices = 0x1130;
    constexpr std::ptrdiff_t m_iFOV = 0x210;
    constexpr std::ptrdiff_t m_bIsScoped = 0x22A0;
    constexpr std::ptrdiff_t attack = 0x17CD040;
    constexpr std::ptrdiff_t m_iIDEntIndex = 0x13A8;
    constexpr std::ptrdiff_t flFlashDuration = 0x1468;

    // Entity details
    constexpr std::ptrdiff_t m_hPawn = 0x60C;
    constexpr std::ptrdiff_t m_iHealth = 0x324;
    constexpr std::ptrdiff_t m_iTeamNum = 0x3C3;
    constexpr std::ptrdiff_t m_vOldOrigin = 0x1274;
    constexpr std::ptrdiff_t m_entitySpottedState = 0x10F8;
    constexpr std::ptrdiff_t m_vecViewOffset = 0xC50;
}


namespace globals {
    // Global settings
    inline int FOV = 90;
    inline bool TriggerBot = false;
    inline int TriggerBotDelay = 20;
    extern int TriggerBotMode;
    extern bool TriggerBotToggled;
    inline std::uintptr_t client = 0;
    inline ImVec4 MenuAccentColor = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    inline bool Rainbow = false;
}
