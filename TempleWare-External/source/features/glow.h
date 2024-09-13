#pragma once

#include "../memory/memory.h"

namespace features {
    class Glow {
    public:
        static void Run(const Memory& memory) noexcept;
    };
}
