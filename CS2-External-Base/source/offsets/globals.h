#pragma once

#include <cstddef>
#include <locale>

#include "../../imgui/imgui.h"

#define VK_LSHIFT 0xA0

namespace globals {
    
    // TriggerBot
    inline bool TriggerBot = false;                    // TriggerBot activation state
    inline int TriggerBotKey = VK_LSHIFT;              // Key to activate TriggerBot
    inline char TriggerBotKeyName[64] = "L-Shift";     // Name of the key for display
    inline int TriggerBotMode = 0;                     // TriggerBot mode (Hold/Toggle)
    inline int TriggerBotDelay = 20;                   // TriggerBot delay in milliseconds
    inline bool TriggerBotToggled = false;             // Toggle state for TriggerBot

    // Fov
    inline int FOV = 90;                               // Field of View setting
    
    // NoFlash
    inline bool NoFlashEnabled = false;                // No Flash activation state

    // Menu
    inline ImVec4 MenuAccentColor = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);  // GUI color setting
    inline bool Rainbow = false;                       // Rainbow color mode activation

    // Application state
    inline bool isRunning = true;                      // Whether the hack is running
    inline std::uintptr_t client = 0;                  // Client base address
}