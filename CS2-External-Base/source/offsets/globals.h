#pragma once

#include <cstddef>
#include <locale>

#include "../../imgui/imgui.h"

#define VK_LSHIFT 0xA0

namespace offsets {
    // Game offsets
    constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x17D37F0;
    constexpr std::ptrdiff_t dwEntityList = 0x19684F8;
    constexpr std::ptrdiff_t dwViewMatrix = 0x19CA480;
    constexpr std::ptrdiff_t dwViewAngles = 0x19D98E8;

    // Player details
    constexpr std::ptrdiff_t m_pCameraServices = 0x1130;
    constexpr std::ptrdiff_t m_iFOV = 0x210;
    constexpr std::ptrdiff_t m_bIsScoped = 0x22A0;
    constexpr std::ptrdiff_t attack = 0x17CC020;
    constexpr std::ptrdiff_t m_iIDEntIndex = 0x13A8;
    constexpr std::ptrdiff_t flFlashDuration = 0x135C;

    // Entity details
    constexpr std::ptrdiff_t m_hPawn = 0x60C;
    constexpr std::ptrdiff_t m_iHealth = 0x324;
    constexpr std::ptrdiff_t m_iTeamNum = 0x3C3;
    constexpr std::ptrdiff_t m_vOldOrigin = 0x1274;
    constexpr std::ptrdiff_t m_entitySpottedState = 0x2288;
    constexpr std::ptrdiff_t m_vecViewOffset = 0xC50;

}


namespace globals {
    // Global settings
    inline int FOV = 90;
    inline bool TriggerBot = false;
    inline int TriggerBotDelay = 20;
    extern int TriggerBotMode;
    extern bool TriggerBotToggled;
    extern int TriggerBotKey;        
    extern char TriggerBotKeyName[64];
    inline std::uintptr_t client = 0;
    inline ImVec4 MenuAccentColor = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    inline bool Rainbow = false;
    inline bool NoFlashEnabled = false;
    inline bool AutoScopeEnabled = false;
}
