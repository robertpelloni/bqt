BOBUI += widgets

HEADERS       = classwizard.h
SOURCES       = classwizard.cpp \
                main.cpp
RESOURCES     = classwizard.qrc

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/dialogs/classwizard
INSTALLS += target
