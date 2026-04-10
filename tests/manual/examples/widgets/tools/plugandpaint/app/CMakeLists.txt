# Copyright (C) 2022 The BobUI Company Ltd.
# SPDX-License-Identifier: LicenseRef-BobUI-Commercial OR BSD-3-Clause

bobui_add_executable(plugandpaint
    interfaces.h
    main.cpp
    mainwindow.cpp mainwindow.h
    paintarea.cpp paintarea.h
    plugindialog.cpp plugindialog.h
)

set_target_properties(plugandpaint PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(plugandpaint PRIVATE
    BobUI6::Widgets
    pnp_basictools
)

if(BOBUI6_IS_SHARED_LIBS_BUILD)
    # Build the shared plugin too when building this example target.
    add_dependencies(plugandpaint pnp_extrafilters)
else()
    # Link the extrafilters plugin if BobUI is built statically.
    target_link_libraries(plugandpaint PRIVATE
        pnp_extrafilters
    )
endif()

install(TARGETS plugandpaint
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)
