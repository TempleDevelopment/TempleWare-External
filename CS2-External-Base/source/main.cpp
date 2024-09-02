/************************************************************************************************
 *                                          Temple Cheat                                        *
 *                     (c) 2023-2024 Temple Client Cheats Team. All rights reserved.            *
 ************************************************************************************************/

#include "menu/GUI.h"
#include "memory/Memory.h"
#include "offsets/Globals.h"
#include "threads/threads.h"

#include <thread>

int __stdcall wWinMain(
    HINSTANCE instance,
    HINSTANCE previousInstance,
    PWSTR arguments,
    int commandShow) {

    const auto memory = Memory("cs2.exe");

    globals::client = memory.GetModuleAddress("client.dll");

    std::thread(threads::RunMiscThread, std::ref(memory)).detach();
    std::thread(threads::RunVisualThread, std::ref(memory)).detach();
    std::thread(threads::RunAimThread, std::ref(memory)).detach();

    gui::CreateHWindow("templecheats.xyz");
    gui::CreateDevice();
    gui::CreateImGui();

    while (globals::isRunning) {
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
