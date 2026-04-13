HEADERS   = block.h \
            renderthread.h \
            window.h
SOURCES   = main.cpp \
            block.cpp \
            renderthread.cpp \
            window.cpp
BOBUI += widgets
requires(bobuiConfig(filedialog))

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/corelib/threads/queuedcustomtype
INSTALLS += target


