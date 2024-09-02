#pragma once

#include "../memory/Memory.h"

namespace features {
    class NoFlash {
    public:
        static void Run(const Memory& memory) noexcept;
    };
}
