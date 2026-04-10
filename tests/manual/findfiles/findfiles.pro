BOBUI += widgets
requires(bobuiConfig(filedialog))

HEADERS       = window.h
SOURCES       = main.cpp \
                window.cpp

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/dialogs/findfiles
INSTALLS += target
