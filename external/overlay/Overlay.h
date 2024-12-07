#pragma once
#include "../../external/imgui/imgui.h"
#include "../../external/imgui/imgui_impl_win32.h"
#include "../../external/imgui/imgui_impl_dx11.h"
#include "../../source/utils/Utils.h"
#include <dwmapi.h>
#include <thread>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

class Overlay
{
private:
	// オーバーレイに登録する情報
	WNDCLASSEXA wc{};
	HWND Hwnd;
	char Title[32] = "The Overlay";
	char Class[32] = "WND_CLS";

	HWND TargetHwnd;
	bool CreateOverlay();
	HWND GetTargetWindow(const std::string processName);	// 実行ファイル名からウィンドウハンドルを取得する
public:
	bool InitOverlay(const std::string targetName, int mode);
	void OverlayLoop();
	void DestroyOverlay();
	void OverlayManager(const std::string targetName);
};

extern ID3D11Device* g_pd3dDevice;
extern ID3D11DeviceContext* g_pd3dDeviceContext;
extern IDXGISwapChain* g_pSwapChain;
extern ID3D11RenderTargetView* g_mainRenderTargetView;
