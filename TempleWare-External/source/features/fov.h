#pragma once

#include "../memory/Memory.h"

namespace features {
    class FOVManager {
    public:
        static void AdjustFOV(const Memory& memory) noexcept;
    };
}
