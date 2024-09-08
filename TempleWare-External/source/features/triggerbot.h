#pragma once

#include "../memory/Memory.h"

namespace features {
    class TriggerBot {
    public:
        static void Run(const Memory& memory) noexcept;
    };
}
