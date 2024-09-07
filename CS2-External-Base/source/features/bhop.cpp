#include "bhop.h"
#include "../offsets/globals.h"
#include "../offsets/offsets.h"

namespace features
{
	void Bhop::BunnyHop(const Memory& memory) noexcept
	{
		if (!globals::Bhop)
			return;

		std::uintptr_t localPlayer = memory.Read<std::uintptr_t>(globals::client + offsets::dwLocalPlayerPawn);
		if (localPlayer == 0)
			return;

		HWND hwnd_cs2 = FindWindowA(NULL, "Counter-Strike 2");
		if (hwnd_cs2 == NULL) {
			hwnd_cs2 = FindWindowA(NULL, "Counter-Strike 2");
		}

		bool spacePressed = GetAsyncKeyState(VK_SPACE);
		int flags = memory.Read<std::uintptr_t>(localPlayer + offsets::m_fFlags);
		bool isInAir = flags & (int)1 << 0;

		if (spacePressed && isInAir)
		{
			SendMessage(hwnd_cs2, WM_KEYUP, VK_SPACE, 0);
			SendMessage(hwnd_cs2, WM_KEYDOWN, VK_SPACE, 0);
		}

		else if (spacePressed && !isInAir)
		{
			SendMessage(hwnd_cs2, WM_KEYUP, VK_SPACE, 0);
		}
		else if (!spacePressed)
		{
			SendMessage(hwnd_cs2, WM_KEYUP, VK_SPACE, 0);
		}
	}
}