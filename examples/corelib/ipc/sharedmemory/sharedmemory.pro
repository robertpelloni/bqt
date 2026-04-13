BOBUI += widgets
requires(bobuiConfig(filedialog))

SOURCES += main.cpp \
           dialog.cpp 

HEADERS += dialog.h

# Forms and resources
FORMS += dialog.ui

EXAMPLE_FILES = *.png

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/corelib/ipc/sharedmemory
INSTALLS += target
