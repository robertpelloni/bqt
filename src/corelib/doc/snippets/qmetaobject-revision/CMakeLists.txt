if(NOT BOBUI_FEATURE_widgets)
    return()
endif()

add_library(corelib_snippets_qmetaobject-revision OBJECT)

set_target_properties(corelib_snippets_qmetaobject-revision PROPERTIES COMPILE_OPTIONS "-w")

target_link_libraries(corelib_snippets_qmetaobject-revision PRIVATE
    BobUI::Core
)

if(BOBUI_FEATURE_widgets)
    target_link_libraries(corelib_snippets_qmetaobject-revision PRIVATE
        BobUI::Widgets
    )
    target_sources(corelib_snippets_qmetaobject-revision PRIVATE
        main.cpp
        window.cpp
    )
endif()
