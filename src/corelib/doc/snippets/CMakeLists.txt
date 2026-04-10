# Copyright (C) 2025 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

add_library(corelib_snippets OBJECT
    customtype/customtypeexample.cpp
    file/file.cpp
    jni/src_qjniobject.cpp
    qbytearraylist/main.cpp
    qdir-listfiles/main.cpp
    qdir-namefilters/main.cpp
    qelapsedtimer/main.cpp
    qloggingcategory/main.cpp
    qmessageauthenticationcode/main.cpp
    qmetatype/registerConverters.cpp
    qrangemodel/main.cpp
    qrangemodeladapter/main.cpp
    qstack/main.cpp
    qstringlist/main.cpp
    qstringlistmodel/main.cpp
    qversionnumber/main.cpp
    qxmlstreamwriter/main.cpp
    settings/settings.cpp
    sharedemployee/main.cpp
    signalsandslots/signalsandslots.cpp
    streaming/main.cpp
    threads/threads.cpp
)

target_link_libraries(corelib_snippets PRIVATE
    BobUI::Core
)

if(BOBUI_FEATURE_process)
    target_sources(corelib_snippets PRIVATE
        process/process.cpp
    )
endif()

if(BOBUI_FEATURE_process AND BOBUI_FEATURE_processenvironment)
    target_sources(corelib_snippets PRIVATE
        qprocess-environment/main.cpp
    )
endif()

if(BOBUI_FEATURE_widgets)
    target_link_libraries(corelib_snippets PRIVATE
        BobUI::Widgets
    )
    target_sources(corelib_snippets PRIVATE
        events/events.cpp
        hellotrmain.cpp
        fileinfo/main.cpp
        pointer/pointer.cpp
        qsignalmapper/buttonwidget.cpp
        qsortfilterproxymodel-details/main.cpp
        qstring/main.cpp
        bobuicast/bobuicast.cpp
        settings/settings.cpp
    )
endif()

if(BOBUI_FEATURE_gui)
    target_link_libraries(corelib_snippets PRIVATE
        BobUI::Gui
    )
    target_sources(corelib_snippets PRIVATE
        buffer/buffer.cpp
        qdebug/qdebugsnippet.cpp
    )
    add_subdirectory(eventfilters)
endif()

set_target_properties(corelib_snippets PROPERTIES COMPILE_OPTIONS "-w")

if ("${CMAKE_CXX_COMPILE_FEATURES}" MATCHES "cxx_std_23")
    set_property(TARGET corelib_snippets PROPERTY CXX_STANDARD 23)
endif()

set_target_properties(corelib_snippets PROPERTIES UNITY_BUILD OFF)

add_subdirectory(qmetaobject-invokable)
add_subdirectory(qmetaobject-revision)
if(BOBUI_FEATURE_process)
    add_subdirectory(qprocess)
endif()
add_subdirectory(code)
