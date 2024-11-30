#!/usr/bin/env python3
import sys
import requests
import json
import re
import os
from pathlib import Path

# URLs for fetching offsets
source_url = "https://raw.githubusercontent.com/a2x/cs2-dumper/main/output/offsets.json"
commits_url = "https://api.github.com/repos/a2x/cs2-dumper/commits"

script_dir = Path(__file__).parent
dest_path = script_dir / "offsets" / "offsets.json"
header_path = script_dir / "TempleWare-External" / "source" / "offsets" / "offsets.h"

# Create directories if they don't exist
os.makedirs(os.path.dirname(dest_path), exist_ok=True)
os.makedirs(os.path.dirname(header_path), exist_ok=True)

# Load existing offsets
with open(dest_path, 'r') as dest_file:
    dest_data = json.load(dest_file)

# Generate offsets.h regardless of updates
header_content = """#pragma once
#include <cstdint>

namespace offsets {
    inline constexpr auto build_number = %(build_number)d;
    inline constexpr auto dwBuildNumber = 0x%(dwBuildNumber)X;
    inline constexpr auto dwLocalPlayerController = 0x%(dwLocalPlayerController)X;
    inline constexpr auto dwEntityList = 0x%(dwEntityList)X;
    inline constexpr auto dwViewMatrix = 0x%(dwViewMatrix)X;
    inline constexpr auto dwPlantedC4 = 0x%(dwPlantedC4)X;
    inline constexpr auto m_iHealth = 0x%(m_iHealth)X;
    inline constexpr auto m_iTeamNum = 0x%(m_iTeamNum)X;
    inline constexpr auto m_Glow = 0xBA0;
    inline constexpr auto m_glowColorOverride = 0x1B78;
    inline constexpr auto m_bGlowing = 0x1B80;
    inline constexpr auto m_flFlashDuration = 0x1468;
    inline constexpr auto m_iIDEntIndex = 0x1544;
    inline constexpr auto m_vOldOrigin = 0x%(m_vOldOrigin)X;
    inline constexpr auto attack = 0x1738B60;
    inline constexpr auto flFlashDuration = 0x1468;
    inline constexpr auto m_pCameraServices = 0x11E0;
    inline constexpr auto m_iFOV = 0x210;
    inline constexpr auto m_bIsScoped = 0x23E8;
    inline constexpr auto m_fFlags = 0x3EC;
    inline constexpr auto m_hPlayerPawn = 0x80C;
}""" % dest_data

# Write the header file
with open(header_path, 'w') as header_file:
    header_file.write(header_content)

print("Header file generated successfully.")
