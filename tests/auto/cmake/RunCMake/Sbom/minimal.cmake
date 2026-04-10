# Needed to make the sbom functions available.
find_package(BobUI6 REQUIRED Core)

include(CommonResultGenIntro)

_bobui_internal_setup_sbom(
    GENERATE_SBOM_DEFAULT "TRUE"
)

# This is used by common_result_gen.cmake.
set(SBOM_VERSION "1.0.0")

_bobui_internal_sbom_begin_project(
    SUPPLIER "BobUIProjectTest"
    SUPPLIER_URL "https://bobui-project.org/SbomTest"
    VERSION "${SBOM_VERSION}"
)

include(common_targets.cmake)

_bobui_internal_sbom_end_project()

include(CommonResultGen)

# Also create separate sboms for some sibling projects under this subdir.
add_subdirectory(subprojects)
