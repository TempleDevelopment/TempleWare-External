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

    bool windowVisible = true;

    while (globals::isRunning) {
        if (GetAsyncKeyState(VK_END) & 0x8000) {
            windowVisible = !windowVisible;
            ShowWindow(gui::window, windowVisible ? SW_SHOW : SW_HIDE);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }

        if (windowVisible) {
            gui::BeginRender();
            gui::Render();
            gui::EndRender();
        }
        else {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }

    gui::DestroyImGui();
    gui::DestroyDevice();
    gui::DestroyHWindow();

    return EXIT_SUCCESS;
}
