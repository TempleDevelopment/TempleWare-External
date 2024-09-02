#include "fov.h"
#include "../offsets/globals.h"
#include "../offsets/offsets.h"

namespace features {
    void FOVManager::AdjustFOV(const Memory& memory) noexcept {
        std::uint16_t desiredFov = static_cast<uint16_t>(globals::FOV);
        std::uintptr_t localPlayer = memory.Read<std::uintptr_t>(globals::client + offsets::dwLocalPlayerPawn);
        std::uintptr_t cameraServices = memory.Read<std::uintptr_t>(localPlayer + offsets::m_pCameraServices);

        std::uint16_t currentFov = memory.Read<std::uint64_t>(cameraServices + offsets::m_iFOV);

        bool isScoped = memory.Read<bool>(localPlayer + offsets::m_bIsScoped);

        if (!isScoped && currentFov != desiredFov) {
            memory.Write<uint64_t>(cameraServices + offsets::m_iFOV, desiredFov);
        }
    }
}
