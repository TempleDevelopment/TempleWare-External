#pragma once

#include <d3d9.h>

namespace gui
{

    constexpr int WIDTH = 500;
    constexpr int HEIGHT = 300;

    inline bool isRunning = true;

    inline HWND window = nullptr;
    inline WNDCLASSEX windowClass = { };

    inline POINTS position = { };

    inline PDIRECT3D9 d3d = nullptr;
    inline LPDIRECT3DDEVICE9 device = nullptr;
    inline D3DPRESENT_PARAMETERS presentParameters = { };

    void CreateHWindow(const char* windowName) noexcept;
    void DestroyHWindow() noexcept;

    bool CreateDevice() noexcept;
    void ResetDevice() noexcept;
    void DestroyDevice() noexcept;

    void CreateImGui() noexcept;
    void DestroyImGui() noexcept;

    void SetupImGuiStyle() noexcept;
    void SetupImGuiFonts() noexcept;

    void BeginRender() noexcept;
    void EndRender() noexcept;
    void Render() noexcept;
    void ApplyCustomStyle() noexcept;
}