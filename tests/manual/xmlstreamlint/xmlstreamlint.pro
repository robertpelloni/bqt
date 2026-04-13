CONFIG   += cmdline
BOBUI       -= gui
SOURCES  += main.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/xml/xmlstreamlint
INSTALLS += target
