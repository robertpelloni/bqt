SOURCES += main.cpp composition.cpp
HEADERS += composition.h

SHARED_FOLDER = ../shared

include($$SHARED_FOLDER/shared.pri)

RESOURCES += composition.qrc
BOBUI += widgets

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/painting/composition
INSTALLS += target
