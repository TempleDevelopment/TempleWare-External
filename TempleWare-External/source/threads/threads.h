#pragma once

#include "../memory/Memory.h"

namespace threads {
    void RunMiscThread(const Memory& memory) noexcept;
    void RunVisualThread(const Memory& memory) noexcept;
    void RunAimThread(const Memory& memory) noexcept;
}
