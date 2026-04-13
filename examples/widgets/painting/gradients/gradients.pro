SOURCES += main.cpp gradients.cpp
HEADERS += gradients.h

SHARED_FOLDER = ../shared

include($$SHARED_FOLDER/shared.pri)

RESOURCES += gradients.qrc
BOBUI += widgets

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/painting/gradients
INSTALLS += target

