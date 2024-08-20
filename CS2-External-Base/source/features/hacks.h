#pragma once
#include "../memory/memory.h"
#include "../features/triggerbot.h"

namespace hacks
{
	void MiscThread(const Memory& memory) noexcept;
	void VisualThread(const Memory& memory) noexcept;
	void AimThread(const Memory& memory) noexcept;
}
