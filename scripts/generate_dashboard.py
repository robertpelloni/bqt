import os
import configparser
import re

def get_submodules():
    submodules = []
    if os.path.exists('.gitmodules'):
        try:
            config = configparser.ConfigParser()
            config.read('.gitmodules')
            for section in config.sections():
                if section.startswith('submodule '):
                    path = config.get(section, 'path', fallback='')
                    url = config.get(section, 'url', fallback='')
                    submodules.append((path, url))
        except Exception as e:
            print(f"Error parsing .gitmodules: {e}")
    return submodules

def generate_dashboard():
    submodules = get_submodules()

    with open('SUBMODULE_DASHBOARD.md', 'w') as f:
        f.write('# BobQ / OmniUI Submodule Dashboard\n\n')
        f.write('This document lists all active submodules and major dependencies in the Omni-Workspace ecosystem.\n\n')
        f.write('## Active Submodules\n\n')

        if not submodules:
            f.write('*No submodules found in .gitmodules.*\n')
        else:
            for path, url in submodules:
                # Provide a fallback summary based on path names
                desc = "Integrated framework dependency"
                if "juce" in path.lower():
                    desc = "Audio / DSP / Component UI framework utilized for 1:1 parity and interop."
                elif "ultimatepp" in path.lower():
                    desc = "Rapid Application Development C++ toolkit utilized for layout parity and interop."

                f.write(f"### `{path}`\n")
                f.write(f"- **URL:** [{url}]({url})\n")
                f.write(f"- **Purpose:** {desc}\n\n")

        f.write('## Architecture Directories\n\n')
        f.write('- `BobQ/` (formerly `OmniUI/`): The core C++ Native Engine (Qt/Meta-Object driven).\n')
        f.write('- `internal/`: The pure Go rewrite and bindings engine.\n')
        f.write('- `docs/`: Universal AI instructions and architectural deep dives.\n')
        f.write('- `scripts/`: Ecosystem automation scripts.\n\n')
        f.write('---\n*Automatically generated via `python scripts/generate_dashboard.py`*\n')

    print("SUBMODULE_DASHBOARD.md successfully generated.")

if __name__ == '__main__':
    generate_dashboard()
