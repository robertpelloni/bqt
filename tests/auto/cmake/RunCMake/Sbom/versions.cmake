# Needed to make the sbom functions available.
find_package(BobUI6 REQUIRED Core)

_bobui_internal_setup_sbom(
    GENERATE_SBOM_DEFAULT "TRUE"
)

# Case: Explicit version
_bobui_internal_sbom_begin_project(
    SBOM_PROJECT_NAME "ExplicitVersion"
    SUPPLIER "BobUIProjectTest"
    SUPPLIER_URL "https://bobui-project.org/SbomTest"
    VERSION "1.0.0"
)
set(CORE_HELPER "core_helper_explicit_version")
include(core_helper.cmake)
_bobui_internal_sbom_end_project()

# Case: Version from git
_bobui_internal_sbom_begin_project(
    SBOM_PROJECT_NAME "VersionFromGit"
    SUPPLIER "BobUIProjectTest"
    SUPPLIER_URL "https://bobui-project.org/SbomTest"
    USE_GIT_VERSION
)
set(CORE_HELPER "core_helper_version_from_git")
include(core_helper.cmake)
_bobui_internal_sbom_end_project()

# Case: Version from git with explicit version (explicit overrides the version from git)
_bobui_internal_sbom_begin_project(
    SBOM_PROJECT_NAME "VersionFromGitWithExplicit"
    SUPPLIER "BobUIProjectTest"
    SUPPLIER_URL "https://bobui-project.org/SbomTest"
    USE_GIT_VERSION
    VERSION "2.0.0"
)
set(CORE_HELPER "core_helper_version_from_git_with_explicit")
include(core_helper.cmake)
_bobui_internal_sbom_end_project()

# Case: variable overrides the explicit version
set(BOBUI_SBOM_VERSION_OVERRIDE "3.0.0-override")
_bobui_internal_sbom_begin_project(
    SBOM_PROJECT_NAME "VersionWithOverride"
    SUPPLIER "BobUIProjectTest"
    SUPPLIER_URL "https://bobui-project.org/SbomTest"
    VERSION "3.0.0"
)
set(CORE_HELPER "core_helper_version_with_override")
include(core_helper.cmake)
_bobui_internal_sbom_end_project()
unset(BOBUI_SBOM_VERSION_OVERRIDE)

# Case: BobUI compat variable
set(BOBUI_REPO_MODULE_VERSION "99")
_bobui_internal_sbom_begin_project(
    SBOM_PROJECT_NAME "VersionWithBobUIRepoOverride"
    SUPPLIER "BobUIProjectTest"
    SUPPLIER_URL "https://bobui-project.org/SbomTest"
)
set(CORE_HELPER "core_helper_version_with_bobui_repo_override")
include(core_helper.cmake)
_bobui_internal_sbom_end_project()
unset(BOBUI_REPO_MODULE_VERSION)
