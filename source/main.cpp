#include "core/framecore.h"
#include "../external/overlay/Overlay.h"

Overlay* C_Overlay = new Overlay;
CFramework* C_FiveM = new CFramework;

#if _DEBUG
int main()
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);

    if (!m.AttachProcess("grcWindow", MODE::WINDOW))
        return 1;

    if (!C_Overlay->InitOverlay("grcWindow", MODE::WINDOW))
        return 2;

    if (!C_FiveM->Init())
        return 3;

    C_Overlay->OverlayLoop();
    C_Overlay->DestroyOverlay();
    m.DetachProcess();
    g.Run = false;

    delete C_FiveM;
    delete C_Overlay;

    return 0;
}

void Overlay::OverlayLoop()
{
    while (g.Run)
    {
        MSG msg;
        while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        C_FiveM->MiscAll();
        OverlayManager("grcWindow");

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        C_FiveM->DrawPerformanceMetrics();

        if (g.WallhackEnabled)
            C_FiveM->DrawEntityOverlays();

        if (g.MenuVisible)
            C_FiveM->DrawModernInterface();

        ImGui::Render();
        const float clear_color_with_alpha[4] = { 0.f, 0.f, 0.f, 0.f };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0);
    }
}
