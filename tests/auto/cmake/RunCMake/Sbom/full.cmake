# Needed to make the sbom functions available.
find_package(BobUI6 REQUIRED Core)

include(CommonResultGenIntro)

_bobui_internal_setup_sbom(
    GENERATE_SBOM_DEFAULT "TRUE"
)

set(IS_FULL_BUILD "TRUE")

# These are used by common_result_gen.cmake.
set(SBOM_VERSION "2.0.0")
set(SBOM_INSTALL_DIR "sbom_full")
set(SBOM_PROJECT_NAME "${PROJECT_NAME}ProjectFull")

_bobui_internal_sbom_begin_project(
    SBOM_PROJECT_NAME "${SBOM_PROJECT_NAME}"
    SUPPLIER "BobUIProjectTest"
    SUPPLIER_URL "https://bobui-project.org/SbomTest"
    LICENSE_EXPRESSION "LGPL-3.0-only"
    COPYRIGHTS "2025 The BobUI Company Ltd."
    INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}"
    INSTALL_SBOM_DIR "${SBOM_INSTALL_DIR}"
    DOWNLOAD_LOCATION "https://download.bobui.io/sbom"
    CPE "cpe:2.3:a:bobui:bobuiprojecttest:1.0.0:*:*:*:*:*:*:*"
    VERSION "${SBOM_VERSION}"
    DOCUMENT_CREATOR_TOOL "Test Build System Tool"
    LICENSE_DIR_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/custom_licenses"
)

include(common_targets.cmake)

_bobui_internal_sbom_end_project()

include(CommonResultGen)

# Also create separate sboms for some sibling projects under this subdir.
add_subdirectory(subprojects)
