HEADERS       = window.h
SOURCES       = main.cpp \
                window.cpp
RESOURCES     = systray.qrc

BOBUI += widgets
requires(bobuiConfig(combobox))

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/desktop/systray
INSTALLS += target
