#pragma once

#include <locale>

namespace offsets {
    // Game offsets
    constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x17D37F0;
    constexpr std::ptrdiff_t dwEntityList = 0x19684F8;
    constexpr std::ptrdiff_t dwViewMatrix = 0x19CA480;
    constexpr std::ptrdiff_t dwViewAngles = 0x19D98E8;
    constexpr std::ptrdiff_t m_vecOrigin = 0x548;

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
