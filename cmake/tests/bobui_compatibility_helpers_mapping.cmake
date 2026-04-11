include("${CMAKE_CURRENT_LIST_DIR}/../BobUICompatibilityHelpers.cmake")

function(_assert_equal actual expected description)
    if(NOT "${actual}" STREQUAL "${expected}")
        message(FATAL_ERROR "${description}: expected '${expected}', got '${actual}'")
    endif()
endfunction()

bobui_compute_qt_package_name(_pkg "BobUI6")
_assert_equal("${_pkg}" "Qt6" "BobUI6 should map to Qt6")

bobui_compute_qt_package_name(_pkg "BobUI6Core")
_assert_equal("${_pkg}" "Qt6Core" "BobUI6Core should map to Qt6Core")

bobui_compute_qt_package_name(_pkg "BobUICore")
_assert_equal("${_pkg}" "QtCore" "BobUICore should map to QtCore")

bobui_compute_qt_package_name(_pkg "Qt6Widgets")
_assert_equal("${_pkg}" "Qt6Widgets" "Non-BobUI package names should remain unchanged")

set(BobUI6_FIND_VERSION "6.12.0")
set(BobUI6_FIND_VERSION_EXACT TRUE)
set(BobUI6_FIND_QUIETLY TRUE)
set(BobUI6_FIND_REQUIRED TRUE)
set(BobUI6_FIND_COMPONENTS Core Widgets)

bobui_collect_forward_find_package_args(_forward_args BobUI6)
list(JOIN _forward_args ";" _joined_args)
_assert_equal("${_joined_args}" "CONFIG;6.12.0;EXACT;QUIET;REQUIRED;COMPONENTS;Core;Widgets" "Forwarded find_package arguments should preserve version/quiet/required/components")

message(STATUS "BobUI compatibility helper mapping checks passed")
