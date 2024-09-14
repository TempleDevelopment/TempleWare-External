#include "gui.h"

#include "../offsets/globals.h"

#include "../../imgui/imgui.h"
#include "../../imgui/imgui_impl_dx9.h"
#include "../../imgui/imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
    HWND window,
    UINT message,
    WPARAM wideParameter,
    LPARAM longParameter
);

LRESULT CALLBACK WindowProcess(
    HWND window,
    UINT message,
    WPARAM wideParameter,
    LPARAM longParameter)
{
    if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter))
        return true;

    switch (message)
    {
    case WM_SIZE: {
        if (gui::device && wideParameter != SIZE_MINIMIZED)
        {
            gui::presentParameters.BackBufferWidth = LOWORD(longParameter);
            gui::presentParameters.BackBufferHeight = HIWORD(longParameter);
            gui::ResetDevice();
        }
        return 0;
    }

    case WM_SYSCOMMAND: {
        if ((wideParameter & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    }

    case WM_DESTROY: {
        PostQuitMessage(0);
        return 0;
    }

    case WM_LBUTTONDOWN: {
        gui::position = MAKEPOINTS(longParameter);
        return 0;
    }

    case WM_MOUSEMOVE: {
        if (wideParameter == MK_LBUTTON)
        {
            const auto points = MAKEPOINTS(longParameter);
            auto rect = RECT{};

            GetWindowRect(gui::window, &rect);

            rect.left += points.x - gui::position.x;
            rect.top += points.y - gui::position.y;

            if (gui::position.x >= 0 &&
                gui::position.x <= gui::WIDTH &&
                gui::position.y >= 0 && gui::position.y <= 19)
                SetWindowPos(
                    gui::window,
                    HWND_TOPMOST,
                    rect.left,
                    rect.top,
                    0, 0,
                    SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER
                );
        }
        return 0;
    }

    }

    return DefWindowProc(window, message, wideParameter, longParameter);
}

void gui::CreateHWindow(const char* windowName) noexcept
{
    WNDCLASSEXA windowClass = { sizeof(WNDCLASSEXA), CS_CLASSDC, WindowProcess, 0L, 0L,
                                GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr,
                                "class001", nullptr };
    RegisterClassExA(&windowClass);

    window = CreateWindowExA(
        0,
        "class001",
        windowName,
        WS_POPUP,
        100,
        100,
        WIDTH,
        HEIGHT,
        0,
        0,
        windowClass.hInstance,
        0
    );

    SetWindowPos(window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);

    ShowWindow(window, SW_SHOWDEFAULT);
    UpdateWindow(window);
}


void gui::DestroyHWindow() noexcept
{
    DestroyWindow(window);
    UnregisterClassA("class001", GetModuleHandle(nullptr));
}

bool gui::CreateDevice() noexcept
{
    d3d = Direct3DCreate9(D3D_SDK_VERSION);

    if (!d3d)
        return false;

    ZeroMemory(&presentParameters, sizeof(presentParameters));

    presentParameters.Windowed = TRUE;
    presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
    presentParameters.EnableAutoDepthStencil = TRUE;
    presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
    presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

    if (d3d->CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        window,
        D3DCREATE_HARDWARE_VERTEXPROCESSING,
        &presentParameters,
        &device) < 0)
        return false;

    return true;
}

void gui::ResetDevice() noexcept
{
    ImGui_ImplDX9_InvalidateDeviceObjects();

    const auto result = device->Reset(&presentParameters);

    if (result == D3DERR_INVALIDCALL)
        IM_ASSERT(0);

    ImGui_ImplDX9_CreateDeviceObjects();
}

void gui::DestroyDevice() noexcept
{
    if (device)
    {
        device->Release();
        device = nullptr;
    }

    if (d3d)
    {
        d3d->Release();
        d3d = nullptr;
    }
}

void gui::CreateImGui() noexcept
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = NULL;
    ImGui::StyleColorsDark();

    const char* fontPath = "C:\\Windows\\Fonts\\segoeui.ttf";
    float fontSize = 18.0f;

    if (!io.Fonts->AddFontFromFileTTF(fontPath, fontSize)) {
        io.Fonts->AddFontDefault();
    }

    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX9_Init(device);
}



void gui::DestroyImGui() noexcept
{
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void gui::BeginRender() noexcept
{
    MSG message;
    while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);

        if (message.message == WM_QUIT)
        {
            isRunning = !isRunning;
            return;
        }
    }

    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void gui::EndRender() noexcept
{
    ImGui::EndFrame();

    device->SetRenderState(D3DRS_ZENABLE, FALSE);
    device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

    device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

    if (device->BeginScene() >= 0)
    {
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
        device->EndScene();
    }

    const auto result = device->Present(0, 0, 0, 0);

    if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
        ResetDevice();
}

void gui::SetupImGuiStyle() noexcept {
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    // Window background
    colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.15f, 1.00f);

    // Headers
    colors[ImGuiCol_Header] = ImVec4(0.20f, 0.25f, 0.30f, 0.55f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);

    // Buttons
    colors[ImGuiCol_Button] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);

    // Frame background
    colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.21f, 0.22f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.38f, 0.38f, 0.38f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);

    // Scrollbar
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.39f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.09f, 0.21f, 0.31f, 1.00f);

    // Resize borders
    style->WindowRounding = 5.3f;
    style->FrameRounding = 2.3f;
    style->ScrollbarRounding = 2.0f;
    style->GrabRounding = 2.0f;
}

std::string GetKeyName(int vk) {
    UINT scanCode = MapVirtualKey(vk, MAPVK_VK_TO_VSC);
    char keyName[128];
    int result = GetKeyNameTextA(scanCode << 16, keyName, sizeof(keyName));
    if (result > 0) {
        return std::string(keyName);
    }
    else {
        return "Unknown";
    }
}

void gui::Render() noexcept {
    static int currentTab = 0;

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(540, 295), ImGuiCond_Always);
    ImGui::Begin(
        "CS2 External | templecheats.xyz",
        &globals::isRunning,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar
    );

    ImGui::PushStyleColor(ImGuiCol_Border, ImColor(0, 0, 0, 255).Value);
    ImGui::BeginChild("##LeftSide", ImVec2(120, ImGui::GetContentRegionAvail().y), true);
    {
        ImGui::Text("CS2 External");
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0.0f, 5.0f));

        if (ImGui::Button("Combat", ImVec2(ImGui::GetContentRegionAvail().x, 30))) currentTab = 0;
        ImGui::Dummy(ImVec2(0.0f, 5.0f));
        if (ImGui::Button("Visual", ImVec2(ImGui::GetContentRegionAvail().x, 30))) currentTab = 1;
        ImGui::Dummy(ImVec2(0.0f, 5.0f));
        if (ImGui::Button("Misc", ImVec2(ImGui::GetContentRegionAvail().x, 30))) currentTab = 2;
        ImGui::Dummy(ImVec2(0.0f, ImGui::GetContentRegionAvail().y - 40));
        if (ImGui::Button("Exit", ImVec2(ImGui::GetContentRegionAvail().x, 30))) {
            exit(0);
        }
    }
    ImGui::EndChild();

    ImGui::SameLine();
    ImGui::BeginChild("##RightSide", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true);
    {
        if (currentTab == 0) {
            // Combat Tab:
            ImGui::Text("Combat");
            ImGui::Separator();
            ImGui::Text("TriggerBot");
            ImGui::SameLine();
            ImGui::Checkbox("##TriggerBotEnable", &globals::TriggerBot);

            if (globals::TriggerBot) {
                ImGui::Text("Key:");
                ImGui::SameLine();
                if (ImGui::Button(globals::TriggerBotKeyName)) {
                    ImGui::OpenPopup("Select Key");
                }

                if (ImGui::BeginPopup("Select Key")) {
                    ImGuiIO& io = ImGui::GetIO();
                    for (int i = 0; i < 256; i++) {
                        if (ImGui::IsKeyPressed(i)) {
                            globals::TriggerBotKey = i;
                            std::string keyName = GetKeyName(i);
                            snprintf(globals::TriggerBotKeyName, sizeof(globals::TriggerBotKeyName), "%s", keyName.c_str());
                            ImGui::CloseCurrentPopup();
                        }
                    }
                    ImGui::Text("Press a key to select.");
                    ImGui::EndPopup();
                }

                if (ImGui::CollapsingHeader("Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
                    ImGui::PushItemWidth(150);
                    const char* modeItems[] = { "Hold", "Toggle" };
                    ImGui::Combo("Mode", &globals::TriggerBotMode, modeItems, IM_ARRAYSIZE(modeItems));
                    ImGui::PopItemWidth();

                    ImGui::SliderInt("Delay (ms)", &globals::TriggerBotDelay, 1, 1000);
                    ImGui::Checkbox("TeamCheck", &globals::TriggerBotTeamCheck);
                    ImGui::Checkbox("IgnoreFlash", &globals::TriggerBotIgnoreFlash);
                }
            }
            ImGui::Separator();
        }

        else if (currentTab == 1) {
            // Visual Tab:
            ImGui::Text("Visual");
            ImGui::Separator();
            ImGui::SliderInt("FOV", &globals::FOV, 0, 160, "FOV: %d");
            ImGui::Separator();
            ImGui::Checkbox("NoFlash", &globals::NoFlashEnabled);
            ImGui::Separator();
        }

        else if (currentTab == 2) {
            // Misc Tab:
            ImGui::Text("Misc");
            ImGui::Separator();
            ImGui::Text("Menu Color");
            ImGui::SameLine();
            if (ImGui::ColorEdit4("##AccentColor", (float*)&globals::MenuAccentColor, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_NoOptions)) {
                globals::Rainbow = false;
            }
            ImGui::Checkbox("Rainbow", &globals::Rainbow);
            ImGui::Separator();
            ImGui::Checkbox("BunnyHop", &globals::BunnyHopEnabled);
            ImGui::Separator();
            ImGui::Checkbox("Bomb Timer", &globals::BombTimerEnabled);
            ImGui::Separator();


            if (globals::Rainbow) {
                static float hue = 0.0f;
                hue += ImGui::GetIO().DeltaTime * 0.1f;
                if (hue > 1.0f) hue = 0.0f;
                ImVec4 rainbowColor = ImColor::HSV(hue, 1.0f, 1.0f);
                globals::MenuAccentColor = rainbowColor;
            }

            ApplyCustomStyle();
        }
    }
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::End();
}

void gui::ApplyCustomStyle() noexcept {
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;
    ImVec4 accentColor = globals::MenuAccentColor;

    colors[ImGuiCol_Header] = accentColor;
    colors[ImGuiCol_HeaderHovered] = accentColor;
    colors[ImGuiCol_HeaderActive] = accentColor;
    colors[ImGuiCol_Button] = accentColor;
    colors[ImGuiCol_ButtonHovered] = accentColor;
    colors[ImGuiCol_ButtonActive] = accentColor;
    colors[ImGuiCol_Tab] = accentColor;
    colors[ImGuiCol_TabHovered] = accentColor;
    colors[ImGuiCol_TabActive] = accentColor;
    colors[ImGuiCol_TabUnfocused] = accentColor;
    colors[ImGuiCol_TabUnfocusedActive] = accentColor;
    colors[ImGuiCol_ScrollbarGrab] = accentColor;
    colors[ImGuiCol_ScrollbarGrabHovered] = accentColor;
    colors[ImGuiCol_ScrollbarGrabActive] = accentColor;
    colors[ImGuiCol_TitleBg] = accentColor;
    colors[ImGuiCol_TitleBgActive] = accentColor;
    colors[ImGuiCol_CheckMark] = accentColor;
    colors[ImGuiCol_SliderGrab] = accentColor;
    colors[ImGuiCol_SliderGrabActive] = accentColor;
}
