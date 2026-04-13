if(NOT BOBUI_FEATURE_widgets)
    return()
endif()

add_library(corelib_snippets_qmetaobject-invokable OBJECT)

set_target_properties(corelib_snippets_qmetaobject-invokable PROPERTIES COMPILE_OPTIONS "-w")

target_link_libraries(corelib_snippets_qmetaobject-invokable PRIVATE
    BobUI::Core
)

if(BOBUI_FEATURE_widgets)
    target_link_libraries(corelib_snippets_qmetaobject-invokable PRIVATE
        BobUI::Widgets
    )
    target_sources(corelib_snippets_qmetaobject-invokable PRIVATE
        main.cpp
        window.cpp
    )
endif()
