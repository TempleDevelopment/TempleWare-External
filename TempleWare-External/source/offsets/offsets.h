#pragma once

#include <locale>

namespace offsets {
    // Game offsets
    constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x17C17F0;
    constexpr std::ptrdiff_t dwLocalPlayerController = 0x19A41C8;
    constexpr std::ptrdiff_t dwEntityList = 0x1954568;
    constexpr std::ptrdiff_t dwViewMatrix = 0x19B64F0;
    constexpr std::ptrdiff_t dwViewAngles = 0x19C5958;
    constexpr std::ptrdiff_t m_vecOrigin = 0x548;

    // Player details
    constexpr std::ptrdiff_t m_pCameraServices = 0x1130;
    constexpr std::ptrdiff_t m_glowColorOverride = 0x40;
    constexpr std::ptrdiff_t m_iFOV = 0x210;
    constexpr std::ptrdiff_t m_bGlowing = 0x51;
    constexpr std::ptrdiff_t m_bIsScoped = 0x22A0;
    constexpr std::ptrdiff_t attack = 0x17BA020;
    constexpr std::ptrdiff_t m_iIDEntIndex = 0x13A8;
    constexpr std::ptrdiff_t flFlashDuration = 0x135C;

    // Entity details
    constexpr std::ptrdiff_t m_hPawn = 0x60C;
    constexpr std::ptrdiff_t m_hPlayerPawn = 0x7EC;
    constexpr std::ptrdiff_t m_Glow = 0xBA0;
    constexpr std::ptrdiff_t m_iHealth = 0x324;
    constexpr std::ptrdiff_t m_iTeamNum = 0x3C3;
    constexpr std::ptrdiff_t m_vOldOrigin = 0x1274;
    constexpr std::ptrdiff_t m_entitySpottedState = 0x2288;
    constexpr std::ptrdiff_t m_vecViewOffset = 0xC50;
    constexpr std::ptrdiff_t m_fFlags = 0x63;
}
