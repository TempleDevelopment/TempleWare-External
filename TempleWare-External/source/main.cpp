/************************************************************************************************
 *                                          Temple Cheat                                        *
 *                     (c) 2023-2024 Temple Client Cheats Team. All rights reserved.            *
 ************************************************************************************************/

#include "menu/GUI.h"
#include "memory/Memory.h"
#include "offsets/Globals.h"
#include "threads/threads.h"

#include <thread>
#include <cstdlib>
#include <filesystem>
#include <iostream>

int __stdcall wWinMain(
    HINSTANCE instance,
    HINSTANCE previousInstance,
    PWSTR arguments,
    int commandShow) {

    // Update offsets before initializing memory
    const char* updateScript = "../../update_offsets.py";
    if (!std::filesystem::exists(updateScript)) {
        MessageBoxA(NULL, "Could not find update_offsets.py", "Offset Update Error", MB_ICONERROR);
        return EXIT_FAILURE;
    }

    if (system((std::string("python \"") + updateScript + "\"").c_str()) != 0) {
        MessageBoxA(NULL, "Failed to update offsets. Using cached values.", "Offset Update Warning", MB_ICONWARNING);
    }

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
