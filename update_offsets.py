#!/usr/bin/env python3
import json
import os
import requests
from pathlib import Path

# Paths
OFFSETS_H_PATH = Path('TempleWare-External/source/offsets/offsets.h')

def fetch_latest_offsets():
    """Fetch latest offsets from cs2-dumper GitHub"""
    try:
        # Fetch client.dll offsets from the raw GitHub content
        response = requests.get('https://raw.githubusercontent.com/a2x/cs2-dumper/master/output/client_dll.json')
        response.raise_for_status()
        return response.json()
    except Exception as e:
        print(f"Error fetching offsets: {e}")
        return None

def process_nested_offsets(data, prefix=""):
    """Process nested offset structure recursively"""
    result = []
    for key, value in sorted(data.items()):
        if isinstance(value, dict):
            # Recursively process nested structures
            nested = process_nested_offsets(value, f"{prefix}{key}_")
            result.extend(nested)
        elif isinstance(value, (int, str)):
            # Convert string hex values to integers
            if isinstance(value, str):
                try:
                    if value.startswith('0x'):
                        value = int(value, 16)
                    else:
                        value = int(value)
                except ValueError:
                    continue
            # Format the offset name to be C++ friendly
            cpp_name = f"{prefix}{key}".upper()
            cpp_name = cpp_name.replace('.', '_').replace('[', '_').replace(']', '')
            result.append((cpp_name, value))
    return result

def update_offsets():
    """Update offsets.h with new values"""
    try:
        data = fetch_latest_offsets()
        if not data:
            return False

        # Create offsets.h content
        content = [
            '#pragma once',
            '#include <cstddef>',
            '',
            'namespace offsets {',
            '    // Auto-generated offsets from cs2-dumper',
            '    // Source: https://github.com/a2x/cs2-dumper',
            ''
        ]

        # Process all offsets recursively
        all_offsets = process_nested_offsets(data)

        # Add each offset with a comment for its hex value
        for name, value in all_offsets:
            content.append(f'    constexpr std::ptrdiff_t {name} = 0x{value:X};  // {hex(value)}')

        content.extend(['', '}  // namespace offsets'])

        # Ensure directory exists
        OFFSETS_H_PATH.parent.mkdir(parents=True, exist_ok=True)

        # Write the file
        with open(OFFSETS_H_PATH, 'w') as f:
            f.write('\n'.join(content))

        print(f"Successfully updated offsets ({len(all_offsets)} offsets written)")
        return True
    except Exception as e:
        print(f"Error updating offsets: {e}")
        return False

if __name__ == "__main__":
    if not update_offsets():
        print("Failed to update offsets")
        exit(1)
