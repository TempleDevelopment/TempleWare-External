#pragma once
#include "../memory/memory.h"
#include <iostream>
#include <Shellapi.h>
#include <thread>

namespace features 
{
	class Bhop 
	{
	public:
		static void BunnyHop(const Memory& memory) noexcept;
	};
}