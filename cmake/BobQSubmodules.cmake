# BobQ Submodule Integration Bridge
# This file isolates third-party submodule CMake configurations to prevent
# variable pollution and macro conflicts with Qt's qt_internal_qtbase_build_repo()

macro(bobq_integrate_submodules)
    message(STATUS "BobQ: Initializing Submodule Interoperability Bridge...")

    # JUCE Integration
    # We must ensure JUCE doesn't forcefully overwrite global compiler flags
    # or conflict with Qt's strict compiler requirements.
    set(JUCE_BUILD_EXAMPLES OFF CACHE BOOL "Disable JUCE examples in BobQ" FORCE)
    set(JUCE_BUILD_EXTRAS OFF CACHE BOOL "Disable JUCE extras in BobQ" FORCE)

    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/submodules/juce/CMakeLists.txt")
        message(STATUS "BobQ: Found JUCE in submodules. Adding directory.")
        # add_subdirectory(submodules/juce EXCLUDE_FROM_ALL)
        # Using EXCLUDE_FROM_ALL ensures it only builds what BobQ specifically links against.
        add_subdirectory(submodules/juce EXCLUDE_FROM_ALL)
    else()
        message(WARNING "BobQ: JUCE submodule not found. Audio parity features will be disabled.")
    endif()

    # Ultimate++ Integration
    # Ultimate++ relies on 'TheIDE' normally. For CMake, we expose its root path
    # so BobQ wrappers can resolve include paths manually via target_include_directories.
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/submodules/ultimatepp")
        message(STATUS "BobQ: Found Ultimate++ in submodules. Exporting UPP_ROOT.")
        set(UPP_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/submodules/ultimatepp" CACHE PATH "Root of Ultimate++ Submodule" FORCE)
    else()
        message(WARNING "BobQ: Ultimate++ submodule not found. C++ layout parity features will be disabled.")
    endif()
endmacro()
