#include "glow.h"
#include "../offsets/globals.h"
#include "../offsets/offsets.h"
#include <thread>

namespace features {
    void Glow::Run(const Memory& memory) noexcept {
        
        if (!globals::Glow)
            return;

        const uintptr_t localPlayerController = memory.Read<uintptr_t>(globals::client + offsets::dwLocalPlayerController);
        if (!localPlayerController)
            return;

        int localTeam = memory.Read<int>(localPlayerController + offsets::m_iTeamNum);

        for (int i = 1; i < 64; i++) {
            uintptr_t entityList = memory.Read<uintptr_t>(globals::client + offsets::dwEntityList);
            if (!entityList)
                continue;

            uintptr_t listEntry = memory.Read<uintptr_t>(entityList + (8 * (i & 0x7FFF) >> 9) + 16);
            if (!listEntry)
                continue;

            uintptr_t player = memory.Read<uintptr_t>(listEntry + 120 * (i & 0x1FF));
            if (!player)
                continue;

            int playerTeam = memory.Read<int>(player + offsets::m_iTeamNum);
            if (playerTeam == localTeam)
                continue;

            uint32_t playerPawn = memory.Read<uint32_t>(player + offsets::m_hPlayerPawn);
            uintptr_t listEntry2 = memory.Read<uintptr_t>(entityList + 0x8 * ((playerPawn & 0x7FFF) >> 9) + 16);
            if (!listEntry2)
                continue;

            uintptr_t playerCsPawn = memory.Read<uintptr_t>(listEntry2 + 120 * (playerPawn & 0x1FF));
            if (!playerCsPawn)
                continue;

            memory.Write<DWORD64>(playerCsPawn + offsets::m_Glow + offsets::m_glowColorOverride, 0x800000FF);
            memory.Write<DWORD64>(playerCsPawn + offsets::m_Glow + offsets::m_bGlowing, 1);
        }
    }
}
