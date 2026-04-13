SOURCES += main.cpp xform.cpp
HEADERS += xform.h

BOBUI += widgets

SHARED_FOLDER = ../shared

include($$SHARED_FOLDER/shared.pri)

RESOURCES += affine.qrc

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/painting/affine
INSTALLS += target
