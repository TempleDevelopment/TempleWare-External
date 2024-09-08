#include "NoFlash.h"
#include "../offsets/globals.h"
#include "../offsets/offsets.h"

namespace features {
    void NoFlash::Run(const Memory& memory) noexcept {
        if (globals::NoFlashEnabled) {
            std::uintptr_t localPlayer = memory.Read<std::uintptr_t>(globals::client + offsets::dwLocalPlayerPawn);
            if (localPlayer) {
                float flashDuration = memory.Read<float>(localPlayer + offsets::flFlashDuration);
                if (flashDuration > 0.0f) {
                    memory.Write<float>(localPlayer + offsets::flFlashDuration, 0.0f);
                }
            }
        }
    }
}
