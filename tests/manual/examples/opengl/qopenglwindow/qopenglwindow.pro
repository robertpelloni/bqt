TEMPLATE = app
TARGET = qopenglwindow
INCLUDEPATH += .
BOBUI += opengl

RESOURCES += shaders.qrc

SOURCES += \
    main.cpp \
    background_renderer.cpp

HEADERS += \
    background_renderer.h

target.path = $$[BOBUI_INSTALL_EXAMPLES]/opengl/qopenglwindow
INSTALLS += target
