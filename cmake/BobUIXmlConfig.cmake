include_guard(GLOBAL)

include("${CMAKE_CURRENT_LIST_DIR}/BobUICompatibilityHelpers.cmake")

bobui_collect_forward_find_package_args(_bobui_find_bobui6xml_args BobUIXml)
find_package(BobUI6Xml ${_bobui_find_bobui6xml_args})

if(BobUI6Xml_FOUND)
    set(BobUIXml_FOUND TRUE)
    set(BobUIXml_VERSION "${BobUI6Xml_VERSION}")
    set(BobUIXml_DIR "${CMAKE_CURRENT_LIST_DIR}")

    _bobui_create_target_forwarder("BobUI::Xml" "BobUI6::Xml")
else()
    set(BobUIXml_FOUND FALSE)
endif()
