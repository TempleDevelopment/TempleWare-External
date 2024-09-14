#pragma once
#include "../memory/memory.h"
#include <iostream>
#include <Shellapi.h>
#include <thread>

namespace features 
{
	class BombTimer
	{
	public:
		static void Run(const Memory& memory) noexcept;
	};
}