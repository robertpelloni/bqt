BOBUI += widgets
requires(bobuiConfig(filedialog))

HEADERS             = screenshot.h
SOURCES             = main.cpp \
                      screenshot.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/desktop/screenshot
INSTALLS += target
