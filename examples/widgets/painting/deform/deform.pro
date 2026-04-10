SOURCES += main.cpp pathdeform.cpp
HEADERS += pathdeform.h

SHARED_FOLDER = ../shared

include($$SHARED_FOLDER/shared.pri)

RESOURCES += deform.qrc

BOBUI += widgets

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/painting/deform
INSTALLS += target
