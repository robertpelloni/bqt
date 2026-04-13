add_library(corelib_snippets_eventfilters OBJECT)

set_target_properties(corelib_snippets_eventfilters PROPERTIES COMPILE_OPTIONS "-w")

target_link_libraries(corelib_snippets_eventfilters PRIVATE
    BobUI::Core
)

if(BOBUI_FEATURE_widgets)
    target_link_libraries(corelib_snippets_eventfilters PRIVATE
        BobUI::Widgets
    )
    target_sources(corelib_snippets_eventfilters PRIVATE
        main.cpp
    )
endif()

if(BOBUI_FEATURE_gui)
    target_link_libraries(corelib_snippets_eventfilters PRIVATE
        BobUI::Gui
    )
    target_sources(corelib_snippets_eventfilters PRIVATE
        filterobject.cpp
    )
endif()
