#pragma once

#include <cstddef>
#include <Windows.h>
#include "../external/imgui/imgui.h"
#include "../memory/memory.h"

namespace features {
    class ESP {
    public:
        static void Run(const Memory& memory) noexcept;
    };
}
