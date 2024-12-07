#pragma once
#include "Config/Config.h"
#include "Memory/Memory.h"
#include "../../external/simplemath/SimpleMath.h"
#include <vector>
#include <thread>
#include <string>
using namespace DirectX::SimpleMath;

extern bool IsKeyDown(int VK);
extern void KeyBinder(int& target_key, int& BindID);
extern const char* KeyNames[];
