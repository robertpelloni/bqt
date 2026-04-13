BOBUI += widgets printsupport

HEADERS       = licensewizard.h
SOURCES       = licensewizard.cpp \
                main.cpp
RESOURCES     = licensewizard.qrc

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/dialogs/licensewizard
INSTALLS += target
