/************************************************************************************************
 *                                          Temple Cheat                                        *
 *                     (c) 2023-2024 Temple Client Cheats Team. All rights reserved.            *
 ************************************************************************************************/

#include "menu/gui.h"
#include "memory.h"
#include "offsets/globals.h"
#include "features/hacks.h"

#include <thread>

int __stdcall wWinMain(
    HINSTANCE instance,
    HINSTANCE previousInstance,
    PWSTR arguments,
    int commandShow) {

    const auto memory = Memory("cs2.exe");

    globals::client = memory.GetModuleAddress("client.dll");

    std::thread(hacks::MiscThread, memory).detach();
    std::thread(hacks::VisualThread, memory).detach();
    std::thread(hacks::AimThread, memory).detach();

    gui::CreateHWindow("templecheats.xyz");
    gui::CreateDevice();
    gui::CreateImGui();

    while (gui::isRunning) {
        gui::BeginRender();
        gui::Render();
        gui::EndRender();

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    gui::DestroyImGui();
    gui::DestroyDevice();
    gui::DestroyHWindow();

    return EXIT_SUCCESS;
}