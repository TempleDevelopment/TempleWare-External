#include "Memory.h"

bool Memory::AttachProcess(const std::string targetName, int mode)
{
    switch (mode)
    {
    case MODE::WINDOW: {
        HWND TargetHwnd = FindWindowA(targetName.c_str(), nullptr); // ClassName

        if (!TargetHwnd) {
            MessageBoxA(nullptr, "FiveM Not Found", "Error", MB_TOPMOST | MB_ICONERROR | MB_OK);
            return false;
        }

        GetWindowThreadProcessId(TargetHwnd, &PID);
        pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);

        if (!pHandle) {
            MessageBoxA(nullptr, "Cannot Open Process", "Error", MB_TOPMOST | MB_ICONERROR | MB_OK);
            return false;
        }

        char pModule[128]{};
        GetModuleBaseNameA(m.pHandle, nullptr, pModule, sizeof(pModule));
        g_BaseAddress = GetModuleBase(pModule);

        if (g_BaseAddress == 0)
            MessageBoxA(nullptr, "Base Adress Is 0", "Warning", MB_OK | MB_TOPMOST);

        return true;
    }
    case MODE::PROCESS: {
        PROCESSENTRY32 process = GetProcess(targetName);

        if (process.th32ProcessID == 0) {
            MessageBoxA(nullptr, "FiveM Not Found", "Error", MB_TOPMOST | MB_ICONERROR | MB_OK);
            return false;
        }

        PID = process.th32ProcessID;
        pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process.th32ProcessID);

        if (!pHandle) {
            MessageBoxA(nullptr, "Cannot Open Process", "Error", MB_TOPMOST | MB_ICONERROR | MB_OK);
            return false;
        }

        char pModule[128]{};
        GetModuleBaseNameA(m.pHandle, nullptr, pModule, sizeof(pModule));
        g_BaseAddress = GetModuleBase(pModule);

        if (g_BaseAddress == 0)
            MessageBoxA(nullptr, "Base Adress Is 0", "Warning", MB_OK | MB_TOPMOST);

        return true;
    }
    default:
        MessageBoxA(nullptr, "Invalid Option", "Error", MB_TOPMOST | MB_ICONERROR | MB_OK);
        return false;
    }
}

void Memory::DetachProcess()
{
    CloseHandle(pHandle);
}

uintptr_t Memory::GetModuleBase(const std::string moduleName)
{
    MODULEENTRY32 entry{};
    entry.dwSize = sizeof(MODULEENTRY32);
    const auto snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, PID);

    while (Module32Next(snapShot, &entry))
    {
        if (!moduleName.compare(entry.szModule))
        {
            CloseHandle(snapShot);
            return reinterpret_cast<uintptr_t>(entry.modBaseAddr);
        }
    }

    if (snapShot)
        CloseHandle(snapShot);

    return reinterpret_cast<uintptr_t>(entry.modBaseAddr);
}

PROCESSENTRY32 Memory::GetProcess(const std::string processName)
{
    PROCESSENTRY32 entry{};
    entry.dwSize = sizeof(PROCESSENTRY32);
    const auto snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    while (Process32Next(snapShot, &entry))
    {
        if (!processName.compare(entry.szExeFile))
        {
            CloseHandle(snapShot);
            return entry;
        }
    }

    CloseHandle(snapShot);

    return PROCESSENTRY32();
}

Memory m;