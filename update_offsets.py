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

# Create offsets directory if it doesn't exist
os.makedirs(os.path.dirname(dest_path), exist_ok=True)

# Fetch the source JSON
source_response = requests.get(source_url)
source_data = source_response.json()

# Fetch build Number
response = requests.get(commits_url)
build_number = 0
if response.status_code == 200:
    commit_data = response.json()
    if commit_data:
        for commit in commit_data:
            commit_message = commit['commit']['message']
            build_match = re.search(r'\bGame [Uu]pdate \((\d+)(?: \(\d+\))?\b', commit_message)
            if build_match:
                build_number = int(build_match.group(1))
                break

# Load or create destination data
if os.path.exists(dest_path):
    with open(dest_path, 'r') as dest_file:
        dest_data = json.load(dest_file)
else:
    dest_data = {"build_number": 0}

if dest_data.get("build_number") == build_number and build_number != 0:
    print("There are no updates in the remote repository")
    sys.exit(0)

dest_data["build_number"] = build_number

# Update only the essential offsets that match the example repository
dest_data["dwBuildNumber"] = source_data["engine2.dll"]["dwBuildNumber"]
dest_data["dwLocalPlayerController"] = source_data["client.dll"]["dwLocalPlayerController"]
dest_data["dwEntityList"] = source_data["client.dll"]["dwEntityList"]
dest_data["dwViewMatrix"] = source_data["client.dll"]["dwViewMatrix"]
dest_data["dwPlantedC4"] = source_data["client.dll"]["dwPlantedC4"]

# Save updated offsets
with open(dest_path, 'w') as dest_file:
    json.dump(dest_data, dest_file, indent=4)

print("Offsets updated in the local file.")
