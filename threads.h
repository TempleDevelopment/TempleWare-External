#pragma once

#include "features/noflash.h"
#include "features/glow.h"
#include "features/esp.h"
#include "features/triggerbot.h"
#include "features/bhop.h"
#include "menu/menu.h"
#include "memory/memory.h"
#include "globals/globals.h"

#include <thread>
#include <chrono>

namespace threads {
    void RunMiscThread(const Memory& memory) noexcept {
        while (gui::isRunning) {
            features::FOVManager::AdjustFOV(memory);
            features::Bhop::Run(memory);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    void RunVisualThread(const Memory& memory) noexcept {
        while (gui::isRunning) {
            features::NoFlash::Run(memory);
            features::Glow::Run(memory);
            features::ESP::Run(memory);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    void RunAimThread(const Memory& memory) noexcept {
        while (gui::isRunning) {
            if (globals::TriggerBot) {
                features::TriggerBot::Run(memory);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}
