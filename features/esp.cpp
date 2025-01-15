#include "esp.h"
#include "../globals/globals.h"
#include "../offsets/offsets.h"
#include <imgui.h>

namespace features {
    void ESP::Run(const Memory& memory) noexcept {
        if (!globals::ESPEnabled)
            return;

        const uintptr_t localPlayerController = memory.Read<uintptr_t>(globals::client + offsets::dwLocalPlayerController);
        if (!localPlayerController)
            return;

        int localTeam = memory.Read<int>(localPlayerController + offsets::m_iTeamNum);

        // Read view matrix for world to screen conversion
        float viewMatrix[16];
        memory.Read(globals::client + offsets::dwViewMatrix, &viewMatrix, sizeof(viewMatrix));

        for (int i = 1; i < 64; i++) {
            uintptr_t entityList = memory.Read<uintptr_t>(globals::client + offsets::dwEntityList);
            if (!entityList)
                continue;

            uintptr_t listEntry = memory.Read<uintptr_t>(entityList + ((8 * (i & 0x7FFF)) >> 9) + 16);
            if (!listEntry)
                continue;

            uintptr_t player = memory.Read<uintptr_t>(listEntry + 120 * (i & 0x1FF));
            if (!player)
                continue;

            int playerTeam = memory.Read<int>(player + offsets::m_iTeamNum);
            if (playerTeam == localTeam)  // Skip teammates
                continue;

            uint32_t playerPawn = memory.Read<uint32_t>(player + offsets::m_hPlayerPawn);
            if (!playerPawn)
                continue;

            uintptr_t listEntry2 = memory.Read<uintptr_t>(entityList + (0x8 * ((playerPawn & 0x7FFF) >> 9)) + 16);
            if (!listEntry2)
                continue;

            uintptr_t playerCsPawn = memory.Read<uintptr_t>(listEntry2 + 120 * (playerPawn & 0x1FF));
            if (!playerCsPawn)
                continue;

            int health = memory.Read<int>(playerCsPawn + offsets::m_iHealth);
            if (health < 1)
                continue;

            // Read player position
            float origin[3];
            memory.Read(playerCsPawn + offsets::m_vOldOrigin, &origin, sizeof(origin));

            // Calculate screen position
            float w = viewMatrix[3] * origin[0] + viewMatrix[7] * origin[1] + viewMatrix[11] * origin[2] + viewMatrix[15];
            if (w < 0.01f)  // Behind camera
                continue;

            float x = viewMatrix[0] * origin[0] + viewMatrix[4] * origin[1] + viewMatrix[8] * origin[2] + viewMatrix[12];
            float y = viewMatrix[1] * origin[0] + viewMatrix[5] * origin[1] + viewMatrix[9] * origin[2] + viewMatrix[13];

            // Convert to screen coordinates
            ImVec2 screen = ImGui::GetIO().DisplaySize;
            float screenX = (screen.x / 2) * (1 + x / w);
            float screenY = (screen.y / 2) * (1 - y / w);

            // Draw ESP box (approximate player size)
            const float playerHeight = 72.0f;  // Approximate player height in game units
            const float playerWidth = playerHeight * 0.4f;

            // Scale based on distance
            float scale = 100.0f / w;
            float boxHeight = playerHeight * scale;
            float boxWidth = playerWidth * scale;

            ImGui::GetBackgroundDrawList()->AddRect(
                ImVec2(screenX - boxWidth/2, screenY - boxHeight),
                ImVec2(screenX + boxWidth/2, screenY),
                IM_COL32(255, 0, 0, 255),  // Red color
                0.0f,  // Rounding
                0,     // Flags
                2.0f   // Thickness
            );
        }
    }
}
