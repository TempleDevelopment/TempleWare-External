#include "TriggerBot.h"
#include "../offsets/globals.h"
#include "../offsets/offsets.h"
#include <thread>

namespace features {
    void TriggerBot::Run(const Memory& memory) noexcept {
        std::uintptr_t localPlayer = memory.Read<std::uintptr_t>(globals::client + offsets::dwLocalPlayerPawn);
        BYTE team = memory.Read<BYTE>(localPlayer + offsets::m_iTeamNum);
        std::uintptr_t entityList = memory.Read<std::uintptr_t>(globals::client + offsets::dwEntityList);

        int crosshairEntityIndex = memory.Read<int>(localPlayer + offsets::m_iIDEntIndex);
        if (crosshairEntityIndex == 0)
            return;

        std::uintptr_t listEntry = memory.Read<std::uintptr_t>(entityList + 0x8 * (crosshairEntityIndex >> 9) + 0x10);
        std::uintptr_t entity = memory.Read<std::uintptr_t>(listEntry + 120 * (crosshairEntityIndex & 0x1ff));

        if (!entity)
            return;
        if (team == memory.Read<BYTE>(entity + offsets::m_iTeamNum))
            return;
        if (memory.Read<int>(entity + offsets::m_iHealth) <= 0)
            return;

        std::this_thread::sleep_for(std::chrono::milliseconds(globals::TriggerBotDelay));
        memory.Write<int>(globals::client + offsets::attack, 65537);
        std::this_thread::sleep_for(std::chrono::milliseconds(globals::TriggerBotDelay));
        memory.Write<int>(globals::client + offsets::attack, 256);
        std::this_thread::sleep_for(std::chrono::milliseconds(globals::TriggerBotDelay));
    }
}
