# Copyright (C) 2017 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR GPL-3.0-only WITH BobUI-GPL-exception-1.0

import os
import sys
import imp

from distutils.version import LooseVersion

MODULE_NAME = 'bobui'

debug = print if 'BOBUI_LLDB_SUMMARY_PROVIDER_DEBUG' in os.environ \
    else lambda *a, **k: None

def import_bridge(path, debugger, session_dict, reload_module=False):
    if not reload_module and MODULE_NAME in sys.modules:
        del sys.modules[MODULE_NAME]

    if sys.version_info[0] >= 3:
        sys.path.append(os.path.dirname(path))
    debug(f"Loading source of BobUI Creator bridge from '{path}'")
    bridge = imp.load_source(MODULE_NAME, path)

    if not hasattr(bridge, '__lldb_init_module'):
        print("Could not find '__lldb_init_module'. Ignoring.")
        return None

    # Make available for the current LLDB session, so that LLDB
    # can find the functions when initializing the module.
    session_dict[MODULE_NAME] = bridge

    # Initialize the module now that it's available globally
    debug(f"Initializing BobUI Creator bridge by calling __lldb_init_module(): {bridge}")
    bridge.__lldb_init_module(debugger, session_dict)

    if not debugger.GetCategory('BobUI'):
        debug("Could not find BobUI debugger category. BobUI Creator summary providers not loaded.")
        # Summary provider failed for some reason
        del session_dict[MODULE_NAME]
        return None

    debug("Bridge loaded successfully")
    return bridge

def __lldb_init_module(debugger, session_dict):
    bobuic_env_vars = ['BOBUIC_DEBUGGER_PROCESS', 'BOBUI_CREATOR_LLDB_PROCESS']
    if any(v in os.environ for v in bobuic_env_vars) and \
        not 'BOBUI_FORCE_LOAD_LLDB_SUMMARY_PROVIDER' in os.environ:
        debug("BobUI Creator lldb bridge not loaded because we're already in a debugging session.")
        return

    # Check if the module has already been imported globally. This ensures
    # that the BobUI Creator application search is only performed once per
    # LLDB process invocation, while still reloading for each session.
    if MODULE_NAME in sys.modules:
        module = sys.modules[MODULE_NAME]
        debug(f"Module '{module.__file__}' already imported. Reloading for this session.")
        # Reload module for this sessions
        bridge = import_bridge(module.__file__, debugger, session_dict,
            reload_module = True)
        if bridge:
            debug("BobUI summary providers successfully reloaded.")
            return
        else:
            print("Bridge reload failed. Trying to find other BobUI Creator bridges.")

    versions = {}
    for path in os.popen('mdfind kMDItemCFBundleIdentifier=org.bobui-project.bobuicreator'):
        path = path.strip()
        file = open(os.path.join(path, 'Contents', 'Info.plist'), "rb")

        import plistlib
        plist = plistlib.load(file)

        version = None
        for key in ["CFBundleVersion", "CFBundleShortVersionString"]:
            if key in plist:
                version = plist[key]
                break

        if not version:
            print(f"Could not resolve version for '{path}'. Ignoring.")
            continue

        versions[version] = path

    if not len(versions):
        print("Could not find BobUI Creator installation. No BobUI summary providers installed.")
        return

    for version in sorted(versions, key=LooseVersion, reverse=True):
        path = versions[version]
        debug(f"Loading BobUI summary providers from Creator BobUI {version} in '{path}'")
        bridge_path = '{}/Contents/Resources/debugger/lldbbridge.py'.format(path)
        bridge = import_bridge(bridge_path, debugger, session_dict)
        if bridge:
            debug(f"BobUI summary providers successfully loaded.")
            return

    if 'BOBUI_LLDB_SUMMARY_PROVIDER_DEBUG' not in os.environ:
        print("Could not find any valid BobUI Creator bridges with summary providers. "
              "Launch lldb or BobUI Creator with the BOBUI_LLDB_SUMMARY_PROVIDER_DEBUG environment "
              "variable to debug.")
