BOBUI += widgets
requires(bobuiConfig(completer))

HEADERS   = fsmodel.h \
            mainwindow.h
SOURCES   = fsmodel.cpp \
            main.cpp \
            mainwindow.cpp
RESOURCES = completer.qrc

# install
target.path = $$[BOBUI_INSTALL_EXAMPLES]/widgets/tools/completer
INSTALLS += target
