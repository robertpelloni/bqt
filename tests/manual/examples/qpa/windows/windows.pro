CONFIG += console
BOBUI += gui-private core-private

HEADERS += window.h
SOURCES += window.cpp main.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/qpa/windows
INSTALLS += target
