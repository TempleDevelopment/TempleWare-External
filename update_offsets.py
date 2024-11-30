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

try:
    # Fetch latest offsets
    response = requests.get(source_url)
    response.raise_for_status()
    source_data = response.json()

    # Extract required offsets
    client_dll = source_data.get("client.dll", {})
    interfaces = client_dll.get("interfaces", {})

    new_offsets = {
        "build_number": source_data.get("timestamp", 0),
        "dwBuildNumber": client_dll.get("data", {}).get("dwBuildNumber", 0),
        "dwEntityList": client_dll.get("data", {}).get("dwEntityList", 0),
        "dwLocalPlayerController": client_dll.get("data", {}).get("dwLocalPlayerController", 0),
        "dwLocalPlayer": client_dll.get("data", {}).get("dwLocalPlayerPawn", 0),  # Note: using dwLocalPlayerPawn for dwLocalPlayer
        "dwViewMatrix": client_dll.get("data", {}).get("dwViewMatrix", 0),
        "dwPlantedC4": client_dll.get("data", {}).get("dwPlantedC4", 0),
        "m_iHealth": client_dll.get("data", {}).get("m_iHealth", 0),
        "m_iTeamNum": client_dll.get("data", {}).get("m_iTeamNum", 0),
        "m_vOldOrigin": client_dll.get("data", {}).get("m_vOldOrigin", 0),
        "m_hPlayerPawn": client_dll.get("data", {}).get("m_hPlayerPawn", 0)
    }

    # Save new offsets
    with open(dest_path, 'w') as dest_file:
        json.dump(new_offsets, dest_file, indent=4)

except Exception as e:
    print(f"Error fetching new offsets: {e}")
    # If fetching fails, try to load existing offsets
    try:
        with open(dest_path, 'r') as dest_file:
            new_offsets = json.load(dest_file)
    except:
        print("Failed to load existing offsets. Using defaults.")
        sys.exit(1)

# Generate offsets.h
header_content = """#pragma once
#include <cstdint>

namespace offsets {
    inline constexpr auto build_number = %(build_number)d;
    inline constexpr auto dwBuildNumber = 0x%(dwBuildNumber)X;
    inline constexpr auto dwLocalPlayerController = 0x%(dwLocalPlayerController)X;
    inline constexpr auto dwLocalPlayer = 0x%(dwLocalPlayer)X;  // Local player pawn
    inline constexpr auto dwEntityList = 0x%(dwEntityList)X;
    inline constexpr auto dwViewMatrix = 0x%(dwViewMatrix)X;
    inline constexpr auto dwPlantedC4 = 0x%(dwPlantedC4)X;
    inline constexpr auto m_iHealth = 0x%(m_iHealth)X;
    inline constexpr auto m_iTeamNum = 0x%(m_iTeamNum)X;
    inline constexpr auto m_vOldOrigin = 0x%(m_vOldOrigin)X;
    inline constexpr auto m_hPlayerPawn = 0x%(m_hPlayerPawn)X;

    // Additional offsets required by the codebase
    inline constexpr auto m_Glow = 0xBA0;
    inline constexpr auto m_glowColorOverride = 0x1B78;
    inline constexpr auto m_bGlowing = 0x1B80;
    inline constexpr auto m_flFlashDuration = 0x1468;
    inline constexpr auto m_iIDEntIndex = 0x1544;
    inline constexpr auto attack = 0x1738B60;
    inline constexpr auto flFlashDuration = 0x1468;
    inline constexpr auto m_pCameraServices = 0x11E0;
    inline constexpr auto m_iFOV = 0x210;
    inline constexpr auto m_bIsScoped = 0x23E8;
    inline constexpr auto m_fFlags = 0x3EC;
}""" % new_offsets

# Write the header file
with open(header_path, 'w') as header_file:
    header_file.write(header_content)

# Clean up any old files
old_paths = [
    script_dir / "source" / "offsets" / "offsets.h",
    script_dir / "source" / "offsets.h",
    script_dir / "TempleWare-External" / "source" / "offsets.h"
]

for old_path in old_paths:
    if old_path.exists():
        os.remove(old_path)

print("Header file generated successfully.")
